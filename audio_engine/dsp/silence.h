#include "dsp/audio_buffer.h"
#include <vector>

std::vector<bool> detect_silence(const AudioBuffer& buffer) {
    std::vector<bool> result;

    const int frame = 1024;
    const float threshold = 0.01f;

    for (size_t i = 0; i < buffer.samples.size(); i += frame) {
        float energy = 0;

        for (size_t j = i; j < i + frame && j < buffer.samples.size(); j++) {
            energy += buffer.samples[j] * buffer.samples[j];
        }

        result.push_back(energy < threshold);
    }

    return result;
}