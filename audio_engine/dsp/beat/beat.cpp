#include <vector>
#include <cmath>
#include <algorithm>
#include "beat.h"
using namespace std;

vector<int> detect_peaks(const vector<float>& onset) {
    vector<int> peaks;

    const int window = 3;

    for (size_t i = window; i < onset.size() - window; i++) {

        float current = onset[i];
        bool isPeak = true;

        for (int j = -window; j <= window; j++) {
            if (onset[i + j] > current) {
                isPeak = false;
                break;
            }
        }

        // threshold (очень важно)
        if (isPeak && current > 0.3f) {
            peaks.push_back(i);
        }
    }

    return peaks;
}

float estimate_phase(
    const vector<int>& peaks,
    float bpm,
    int sampleRate,
    int hop
) {
    if (peaks.empty()) return 0;

    float beatInterval = (60.0f / bpm) * sampleRate / hop;

    float bestOffset = 0;
    float bestScore = 0;

    // перебираем возможные фазы
    for (float offset = 0; offset < beatInterval; offset += 1.0f) {

        float score = 0;

        for (int p : peaks) {
            float dist = fmod((float)p - offset, beatInterval);
            dist = min(dist, beatInterval - dist);

            score += exp(-dist * dist / 10.0f);
        }

        if (score > bestScore) {
            bestScore = score;
            bestOffset = offset;
        }
    }

    return bestOffset;
}

vector<float> build_beat_grid(
    float bpm,
    float phase,
    int totalFrames,
    int sampleRate,
    int hop
) {
    vector<float> beats;

    float beatInterval = (60.0f / bpm) * sampleRate / hop;

    for (float t = phase; t < totalFrames; t += beatInterval) {
        float seconds = t * hop / sampleRate;
        beats.push_back(seconds);
    }

    return beats;
}