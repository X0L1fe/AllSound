#include "dsp/audio_buffer.h"
#include <cmath>

float compute_energy(const AudioBuffer& buffer) {
    double sum = 0.0;

    for (float s : buffer.samples) {
        sum += s * s;
    }

    double rms = sqrt(sum / buffer.samples.size());
    if (buffer.samples.empty()) return 0.0f;
    return (float)rms;
}