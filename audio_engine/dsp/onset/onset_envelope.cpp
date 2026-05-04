#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
#include <dsp/audio_buffer.h>
using namespace std;

// --- normalize ---
static void normalize(vector<float>& v) {
    float maxVal = 0;
    for (float x : v) if (x > maxVal) maxVal = x;

    if (maxVal == 0) return;

    for (float& x : v) x /= maxVal;
}

// --- smoothing ---
static void smooth(vector<float>& v) {
    vector<float> temp = v;

    if (v.size() < 3) return; 

    for (size_t i = 1; i < v.size() - 1; i++) {
        v[i] = (temp[i-1] + temp[i] + temp[i+1]) / 3.0f;
    }
}

// --- DC removal ---
static void remove_dc(vector<float>& v) {
    float mean = 0;
    for (float x : v) mean += x;
    mean /= v.size();
    
    if (v.empty()) return; // чтобы не дал NaN

    for (float& x : v) x -= mean;
}

// --- MAIN FUNCTION ---
const int frame = 1024;
const int hop = 512;
vector<float> compute_onset_envelope(const AudioBuffer& buffer) {
    
    vector<float> envelope;

    //printf("compute_onset_envelope\n");

    for (size_t i = frame; i + frame < buffer.samples.size(); i += hop) {
        float sum = 0;

        for (int j = 0; j < frame; j++) {
            size_t idx1 = i + j;
            size_t idx2 = i + j - hop;

            float diff = buffer.samples[idx1] - buffer.samples[idx2];
            if (diff > 0) sum += diff;
        }

        envelope.push_back(sum);
    }

    
    // ПАЙПЛАЙН

    if (envelope.empty()) return envelope;

    normalize(envelope);
    //printf("normalize\n");

    if (envelope.size() >= 3) {
        smooth(envelope);
        //printf("smooth\n");
    }

    remove_dc(envelope);
    //printf("remove_dc\n");

    return envelope;
}