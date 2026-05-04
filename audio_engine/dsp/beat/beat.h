#pragma once
#include <vector>
using namespace std;

vector<int> detect_peaks(const vector<float>& onset);
vector<float> build_beat_grid(
    float bpm,
    float phase,
    int totalFrames,
    int sampleRate,
    int hop
);

float estimate_phase(
    const std::vector<int>& peaks,
    float bpm,
    int sr,
    int hop
);

vector<float> refine_beats_with_onset(
    const vector<float>& beats,
    const vector<float>& onset,
    int sampleRate,
    int hop
);