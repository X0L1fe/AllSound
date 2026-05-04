#include "phase.h"
#include <cmath>
#include <vector>
using namespace std;

float estimate_phase(
    const vector<int>& peaks,
    float bpm,
    int sampleRate,
    int hop
) {
    if (peaks.empty() || bpm <= 0) return 0;

    float beatInterval = (60.0f / bpm) * sampleRate / hop;

    float bestOffset = 0;
    float bestScore = 0;

    // перебор фаз
    for (float offset = 0; offset < beatInterval; offset += 1.0f) {

        float score = 0;

        for (int p : peaks) {

            float dist = fmod((float)p - offset, beatInterval);
            if (dist < 0) dist += beatInterval;

            dist = min(dist, beatInterval - dist);

            // гауссово окно
            score += exp(-(dist * dist) / (2 * 4.0f));
        }

        if (score > bestScore) {
            bestScore = score;
            bestOffset = offset;
        }
    }

    return bestOffset;
}