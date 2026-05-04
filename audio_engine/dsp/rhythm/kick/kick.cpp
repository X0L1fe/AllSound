#include "kick.h"
#include <cmath>
#include <vector>
using namespace std;

static float low_freq_energy(const vector<float>& frame) {
    float energy = 0;

    // Простейший LPF через разностный сигнал (быстро и без FFT)
    for (size_t i = 1; i < frame.size(); i++) {
        float diff = frame[i] - frame[i - 1];
        energy += fabs(diff);
    }

    return energy;
}

vector<float> detect_kicks(const AudioBuffer& buffer) {
    const int frame = 1024;
    const int hop = 512;

    vector<float> energy_env;

    // --- 1. energy envelope ---
    for (size_t i = 0; i + frame < buffer.samples.size(); i += hop) {
        vector<float> window(frame);

        for (int j = 0; j < frame; j++) {
            window[j] = buffer.samples[i + j];
        }

        energy_env.push_back(low_freq_energy(window));
    }

    // --- 2. flux ---
    vector<float> flux(energy_env.size(), 0);

    for (size_t i = 1; i < energy_env.size(); i++) {
        float diff = energy_env[i] - energy_env[i - 1];
        if (diff > 0) flux[i] = diff;
    }

    // --- 3. normalize ---
    float maxVal = 0;
    for (float v : flux) if (v > maxVal) maxVal = v;
    if (maxVal > 0) {
        for (float& v : flux) v /= maxVal;
    }

    // --- 4. peak picking ---
    vector<float> kicks;

    const float threshold = 0.3f;

    for (size_t i = 1; i < flux.size() - 1; i++) {
        if (flux[i] > threshold &&
            flux[i] > flux[i - 1] &&
            flux[i] > flux[i + 1]) {

            float time = (float)i * hop / buffer.sampleRate;
            kicks.push_back(time);
        }
    }

    return kicks;
}