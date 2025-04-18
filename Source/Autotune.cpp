// AutoTune.cpp

#include "AutoTune.h"
#include <juce_dsp/juce_dsp.h>
#include <map>
#include <cmath>

#include "PitchShifter.h"

AutoTune::AutoTune(const juce::String& keySignature, float dryWetMix_)
    : dryWetMix(dryWetMix_)
{
    generateNotesFromKey(keySignature);
}

AutoTune::~AutoTune() {}

void AutoTune::prepare(double sampleRate, juce::uint32 maxBlockSize, juce::uint32 numChannels)
{
    currentSampleRate = sampleRate;
    currentMaxBlockSize = maxBlockSize;
    currentNumChannels = numChannels;
}

void AutoTune::setDryWetMix(float mix) { dryWetMix = mix; }
float AutoTune::getDryWetMix() const { return dryWetMix; }

void AutoTune::generateNotesFromKey(const juce::String& keySignature)
{
    auto parts = juce::StringArray::fromTokens(keySignature.trim(), " ", "");
    juce::String root = parts[0].toUpperCase();
    juce::String mode = (parts.size() > 1 ? parts[1].toUpperCase() : "MAJOR");

    static const std::map<juce::String, int> noteMap = {
        {"C",0}, {"C#",1}, {"DB",1}, {"D",2}, {"D#",3}, {"EB",3},
        {"E",4}, {"F",5}, {"F#",6}, {"GB",6}, {"G",7},  {"G#",8},
        {"AB",8},{"A",9}, {"A#",10},{"BB",10},{"B",11}
    };

    int rootSemitone = 0;
    auto it = noteMap.find(root);
    if (it != noteMap.end())
        rootSemitone = it->second;

    const std::vector<int> majorIntervals = { 0,2,4,5,7,9,11 };
    const std::vector<int> minorIntervals = { 0,2,3,5,7,8,10 };
    const auto& intervals = (mode == "MINOR" ? minorIntervals : majorIntervals);

    validFrequencies.clear();
    validFrequencies.reserve(128);
    for (int midi = 0; midi < 128; ++midi)
    {
        int noteClass = midi % 12;
        for (int interval : intervals)
        {
            if (noteClass == (rootSemitone + interval) % 12)
            {
                double freq = 440.0 * std::pow(2.0, (midi - 69) / 12.0);
                validFrequencies.push_back(static_cast<float>(freq));
                break;
            }
        }
    }
}

float AutoTune::detectPitch(const float* channelData, int numSamples, double sampleRate)
{
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
        float imag = (i == 0 || i == halfSize)
            ? 0.0f
            : fftData[fftSize - i];
        fftData[i] = real * real + imag * imag;
    }

    std::fill(fftData.begin() + halfSize + 1,
        fftData.begin() + fftSize,
        0.0f);

    fft.performRealOnlyInverseTransform(fftData.data());

    const int minFreq = 50, maxFreq = 1000;
    int minLag = static_cast<int>(sampleRate / maxFreq);
    int maxLag = static_cast<int>(sampleRate / minFreq);
    maxLag = std::min(maxLag, fftSize - 1);

    float maxVal = 0.0f;
    int bestLag = minLag;
    for (int lag = minLag; lag <= maxLag; ++lag)
    {
        if (fftData[lag] > maxVal)
        {
            maxVal = fftData[lag];
            bestLag = lag;
        }
    }

    return bestLag > 0 ? static_cast<float>(sampleRate / bestLag) : 0.0f;
}

float AutoTune::findClosestFrequency(float detectedPitch)
{
    float closest = validFrequencies.front();
    float minDiff = std::abs(detectedPitch - closest);
    for (auto freq : validFrequencies)
    {
        float diff = std::abs(detectedPitch - freq);
        if (diff < minDiff)
        {
            minDiff = diff;
            closest = freq;
        }
    }
    return closest;
}

float AutoTune::calculateShiftFactor(float detectedPitch, float targetFrequency)
{
    return targetFrequency / detectedPitch;
}

void AutoTune::pitchShiftBuffer(juce::AudioBuffer<float>& buffer, float shiftFactor)
{
    juce::dsp::ProcessSpec spec{
        currentSampleRate,
        currentMaxBlockSize,
        currentNumChannels
    };

    juce::dsp::PitchShifter<float> shifter;
    shifter.prepare(spec);
    shifter.setPitchRatio(shiftFactor);

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> ctx(block);
    shifter.process(ctx);
}

void AutoTune::process(juce::AudioBuffer<float>& buffer)
{
    int numSamples = buffer.getNumSamples();
    juce::AudioBuffer<float> temp(buffer);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* data = buffer.getReadPointer(channel);
        float detected = detectPitch(data, numSamples, currentSampleRate);
        if (detected <= 0.0f)
            continue;

        float targetFreq = findClosestFrequency(detected);
        float shift = calculateShiftFactor(detected, targetFreq);

        temp = buffer;
        pitchShiftBuffer(temp, shift);

        auto* tuned = temp.getReadPointer(channel);
        auto* writePtr = buffer.getWritePointer(channel);
        for (int i = 0; i < numSamples; ++i)
            writePtr[i] = dryWetMix * tuned[i] + (1.0f - dryWetMix) * writePtr[i];
    }
}
