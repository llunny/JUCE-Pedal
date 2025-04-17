// Harmony.cpp
#include "Harmony.h"
#include <map>
#include <cmath>

Harmony::Harmony(const juce::String& keySignature, float dryWetMix_)
    : dryWetMix(dryWetMix_)
{
    generateNotesFromKey(keySignature);
}

Harmony::~Harmony() {}

void Harmony::setDryWetMix(float mix) { dryWetMix = mix; }
float Harmony::getDryWetMix() const { return dryWetMix; }

void Harmony::generateNotesFromKey(const juce::String& keySignature)
{
    // Similar to AutoTune: build scaleFrequencies for the requested key
    auto parts = juce::StringArray::fromTokens(keySignature.trim(), " ", "");
    juce::String root = parts[0].toUpperCase();
    juce::String mode = (parts.size() > 1 ? parts[1].toUpperCase() : "MAJOR");

    static const std::map<juce::String,int> noteMap = {
        {"C",0},{"C#",1},{"DB",1},{"D",2},{"D#",3},{"EB",3},
        {"E",4},{"F",5},{"F#",6},{"GB",6},{"G",7},{"G#",8},
        {"AB",8},{"A",9},{"A#",10},{"BB",10},{"B",11}
    };
    int rootSemitone = noteMap.at(root);
    const std::vector<int> majorIntervals = {0,2,4,5,7,9,11};
    const std::vector<int> minorIntervals = {0,2,3,5,7,8,10};
    const auto& intervals = (mode == "MINOR" ? minorIntervals : majorIntervals);

    scaleFrequencies.clear();
    for (int midi = 0; midi < 128; ++midi)
    {
        int noteClass = midi % 12;
        for (int interval : intervals)
        {
            if (noteClass == (rootSemitone + interval) % 12)
            {
                double freq = 440.0 * std::pow(2.0, (midi - 69) / 12.0);
                scaleFrequencies.push_back(static_cast<float>(freq));
                break;
            }
        }
    }
}

float Harmony::detectPitch(const float* channelData, int numSamples, double sampleRate)
{
    // FFT-based autocorrelation as in AutoTune
    int fftOrder = static_cast<int>(std::ceil(std::log2(numSamples * 2)));
    int fftSize = 1 << fftOrder;
    juce::dsp::FFT fft(fftOrder);
    std::vector<float> fftData(fftSize * 2, 0.0f);
    for (int i = 0; i < numSamples; ++i)
        fftData[i] = channelData[i];
    fft.performRealOnlyForwardTransform(fftData.data());
    int halfSize = fftSize / 2;
    for (int i = 0; i <= halfSize; ++i)
    {
        float real = fftData[i];
        float imag = (i == 0 || i == halfSize) ? 0.0f : fftData[fftSize - i];
        fftData[i] = real * real + imag * imag;
    }
    std::fill(fftData.begin() + halfSize + 1, fftData.begin() + fftSize, 0.0f);
    fft.performRealOnlyInverseTransform(fftData.data());
    int minFreq = 50, maxFreq = 1000;
    int minLag = static_cast<int>(sampleRate / maxFreq);
    int maxLag = std::min(static_cast<int>(sampleRate / minFreq), fftSize - 1);
    float maxVal = 0.0f; int bestLag = minLag;
    for (int lag = minLag; lag <= maxLag; ++lag)
        if (fftData[lag] > maxVal) { maxVal = fftData[lag]; bestLag = lag; }
    return bestLag > 0 ? static_cast<float>(sampleRate / bestLag) : 0.0f;
}

void Harmony::process(juce::AudioBuffer<float>& buffer)
{
    int numSamples = buffer.getNumSamples();
    double sampleRate = buffer.getSampleRate();
    juce::AudioBuffer<float> thirdBuf(buffer), fifthBuf(buffer);
    juce::dsp::ProcessSpec spec{ sampleRate, static_cast<uint32>(numSamples), static_cast<uint32>(buffer.getNumChannels()) };
    pitchShifter.prepare(spec);
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* inData = buffer.getReadPointer(channel);
        float detected = detectPitch(inData, numSamples, sampleRate);
        if (detected <= 0.0f) continue;
        int baseIndex = 0;
        for (size_t i = 0; i < scaleFrequencies.size(); ++i)
            if (std::abs(scaleFrequencies[i] - detected) < 1.0f)
                baseIndex = static_cast<int>(i);
        float thirdFreq = scaleFrequencies[(baseIndex + 2) % scaleFrequencies.size()];
        float fifthFreq = scaleFrequencies[(baseIndex + 4) % scaleFrequencies.size()];
        float shift3 = thirdFreq / detected, shift5 = fifthFreq / detected;
        { juce::dsp::AudioBlock<float> b(thirdBuf); juce::dsp::ProcessContextReplacing<float> c(b); pitchShifter.setPitchRatio(shift3); pitchShifter.process(c); }
        { juce::dsp::AudioBlock<float> b(fifthBuf); juce::dsp::ProcessContextReplacing<float> c(b); pitchShifter.setPitchRatio(shift5); pitchShifter.process(c); }
    }
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* w = buffer.getWritePointer(channel);
        auto* h3 = thirdBuf.getReadPointer(channel);
        auto* h5 = fifthBuf.getReadPointer(channel);
        for (int i = 0; i < numSamples; ++i)
            w[i] = (1.0f - dryWetMix) * w[i] + (dryWetMix * 0.5f) * (h3[i] + h5[i]);
    }
}