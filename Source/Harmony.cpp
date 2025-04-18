// Harmony.cpp
#include "Harmony.h"
#include <map>
#include <cmath>

Harmony::Harmony(const juce::String& keySignature, float dryWetMix_)
    : dryWetMix(dryWetMix_)
{
    generateNotesFromKey(keySignature);
}

void Harmony::prepare(double sampleRate,
    juce::uint32 maxBlockSize,
    juce::uint32 numChannels)
{
    currentSampleRate = sampleRate;
    currentMaxBlockSize = maxBlockSize;
    currentNumChannels = numChannels;
}

void Harmony::setDryWetMix(float mix) { dryWetMix = mix; }
float Harmony::getDryWetMix() const { return dryWetMix; }

void Harmony::generateNotesFromKey(const juce::String& keySignature)
{
    auto parts = juce::StringArray::fromTokens(keySignature.trim(), " ", "");
    juce::String root = parts[0].toUpperCase();
    juce::String mode = (parts.size() > 1 ? parts[1].toUpperCase() : "MAJOR");

    static const std::map<juce::String, int> noteMap = {
        {"C",0}, {"C#",1}, {"DB",1}, {"D",2}, {"D#",3}, {"EB",3},
        {"E",4}, {"F",5}, {"F#",6}, {"GB",6}, {"G",7}, {"G#",8},
        {"AB",8},{"A",9}, {"A#",10},{"BB",10},{"B",11}
    };

    int rootSemitone = 0;
    if (auto it = noteMap.find(root); it != noteMap.end())
        rootSemitone = it->second;

    const std::vector<int> majorInts = { 0,2,4,5,7,9,11 },
        minorInts = { 0,2,3,5,7,8,10 };
    auto& intervals = (mode == "MINOR" ? minorInts : majorInts);

    scaleFrequencies.clear();
    scaleFrequencies.reserve(128);

    for (int midi = 0; midi < 128; ++midi)
    {
        int noteClass = midi % 12;
        for (int iv : intervals)
            if (noteClass == (rootSemitone + iv) % 12)
            {
                float freq = 440.0f * std::pow(2.0f, (midi - 69) / 12.0f);
                scaleFrequencies.push_back(freq);
                break;
            }
    }
}

float Harmony::detectPitch(const float* channelData,
    int numSamples,
    double sampleRate)
{
    int fftOrder = (int)std::ceil(std::log2(numSamples * 2));
    int fftSize = 1 << fftOrder;
    juce::dsp::FFT fft(fftOrder);
    std::vector<float> data(fftSize * 2, 0.0f);

    for (int i = 0; i < numSamples; ++i) data[i] = channelData[i];
    fft.performRealOnlyForwardTransform(data.data());

    int half = fftSize / 2;
    for (int i = 0; i <= half; ++i)
    {
        float re = data[i];
        float im = (i == 0 || i == half) ? 0.0f : data[fftSize - i];
        data[i] = re * re + im * im;
    }
    std::fill(data.begin() + half + 1, data.begin() + fftSize, 0.0f);
    fft.performRealOnlyInverseTransform(data.data());

    int minLag = (int)(sampleRate / 1000.0), maxLag = std::min((int)(sampleRate / 50.0), fftSize - 1);
    float bestVal = 0.0f; int bestLag = minLag;
    for (int lag = minLag; lag <= maxLag; ++lag)
        if (data[lag] > bestVal) bestVal = data[lag], bestLag = lag;

    return bestLag > 0 ? (float)(sampleRate / bestLag) : 0.0f;
}

void Harmony::process(juce::AudioBuffer<float>& buffer)
{
    int numSamples = buffer.getNumSamples();
    // copy dry for thirds and fifths
    juce::AudioBuffer<float> buf3(buffer), buf5(buffer);

    juce::dsp::ProcessSpec spec{
        currentSampleRate,
        currentMaxBlockSize,
        currentNumChannels
    };
    pitchShifter.prepare(spec);

    // detect & shift per channel
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        float detected = detectPitch(buffer.getReadPointer(ch), numSamples, currentSampleRate);
        if (detected <= 0.0f) continue;

        // find closest scale degree
        int idx = 0;
        float bestD = std::abs(detected - scaleFrequencies[0]);
        for (int i = 1; i < (int)scaleFrequencies.size(); ++i)
        {
            float d = std::abs(detected - scaleFrequencies[i]);
            if (d < bestD) bestD = d, idx = i;
        }

        float shift3 = scaleFrequencies[(idx + 2) % scaleFrequencies.size()] / detected;
        float shift5 = scaleFrequencies[(idx + 4) % scaleFrequencies.size()] / detected;

        {
            juce::dsp::AudioBlock<float>        b3(buf3);
            juce::dsp::ProcessContextReplacing<float> c3(b3);
            pitchShifter.setPitchRatio(shift3);
            pitchShifter.process(c3);
        }
        {
            juce::dsp::AudioBlock<float>        b5(buf5);
            juce::dsp::ProcessContextReplacing<float> c5(b5);
            pitchShifter.setPitchRatio(shift5);
            pitchShifter.process(c5);
        }
    }

    // mix them back
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* out = buffer.getWritePointer(ch);
        auto* d3 = buf3.getReadPointer(ch);
        auto* d5 = buf5.getReadPointer(ch);

        for (int i = 0; i < numSamples; ++i)
            out[i] = (1.0f - dryWetMix) * out[i]
            + (dryWetMix * 0.5f) * (d3[i] + d5[i]);
    }
}
