// PitchShifter.h
#pragma once

#include <juce_dsp/juce_dsp.h>
#include <cmath>

namespace juce {
    namespace dsp {

        /**
            A very simple delay‑line modulation shifter
            that matches juce::dsp::PitchShifter<float>'s API
        */
        template<typename SampleType>
        class PitchShifter
        {
        public:
            PitchShifter() = default;

            // exactly the same signature as juce::dsp::PitchShifter::prepare
            void prepare(const ProcessSpec& spec)
            {
                // store parameters
                fs = spec.sampleRate;
                bufferSize = static_cast<int>(fs);           // 1 second max-delay
                delayBuffer.setSize((int)spec.numChannels, bufferSize);
                delayBuffer.clear();

                writeIndex = 0;
                pitchRatio = 1.0f;
                delayInSamples = 64.0f;                          // small default
            }

            // same signature as juce::dsp::PitchShifter::setPitchRatio
            void setPitchRatio(float ratio)
            {
                pitchRatio = ratio;
            }

            // same signature as juce::dsp::PitchShifter::process
            void process(ProcessContextReplacing<SampleType> context)
            {
                auto& block = context.getOutputBlock();
                int numCh = (int)block.getNumChannels();
                int numSamps = (int)block.getNumSamples();

                for (int ch = 0; ch < numCh; ++ch)
                {
                    auto* data = block.getChannelPointer(ch);
                    for (int i = 0; i < numSamps; ++i)
                    {
                        // write into our circular buffer
                        delayBuffer.setSample(ch, writeIndex, data[i]);

                        // compute read position with pitchRatio
                        int delayOff = (int)(pitchRatio * delayInSamples);
                        int readPos = (writeIndex - delayOff + bufferSize) % bufferSize;

                        // pull out the delayed sample
                        data[i] = delayBuffer.getSample(ch, readPos);

                        // advance write head
                        writeIndex = (writeIndex + 1) % bufferSize;
                    }
                }
            }

        private:
            AudioBuffer<SampleType> delayBuffer;
            int     bufferSize = 0,
                writeIndex = 0;
            float   fs = 44100.0f,
                pitchRatio = 1.0f,
                delayInSamples = 64.0f;
        };

    } // namespace dsp
} // namespace juce
