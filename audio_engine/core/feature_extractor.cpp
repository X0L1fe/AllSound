#include "feature_extractor.h"
#include "dsp/energy.h"
#include "dsp/silence.h"

#include "dsp/onset/onset.h"
#include "dsp/tempo/bpm.h"
#include "dsp/beat/beat.h"
#include <audio_features.h>
#include "dsp/structure/downbeat.h"
#include "dsp/rhythm/kick/kick.h"


AudioFeatures FeatureExtractor::extract(const AudioBuffer& buffer) {
    AudioFeatures f;

    //printf("Start extract\n");

    f.energy = compute_energy(buffer);
    //printf("Energy done\n");

    f.onset = compute_onset_envelope(buffer);
    //printf("Onset done\n");
    
    f.bpm = detect_bpm(f.onset, buffer.sampleRate, 512);
    //printf("BPM done\n");

    f.period = autocorrelation(f.onset);
    //printf("Autocorr done\n");

    auto peaks = detect_peaks(f.onset);

    float phase = estimate_phase(
        peaks,
        f.bpm,
        buffer.sampleRate,
        512
    );

    // --- GRID ---
    auto grid = build_beat_grid(
        f.bpm,
        phase,
        f.onset.size(),
        buffer.sampleRate,
        512
    );

    // --- KICKS ---
    auto kicks = detect_kicks(buffer);

    // --- ALIGN (snap grid → kicks) ---
    vector<float> aligned;

    for (float g : grid) {
        float best = g;
        float minDist = 0.05f; // 50 ms tolerance

        for (float k : kicks) {
            float d = fabs(k - g);
            if (d < minDist) {
                minDist = d;
                best = k;
            }
        }

        aligned.push_back(best);
    }

f.beats = aligned;

    f.downbeats = detect_downbeats(
        f.beats,
        f.onset,
        buffer.sampleRate,
        512
    );
    
    f.bar_index = detect_bars(f.downbeats);
        f.phrase_index = detect_phrases(
        f.downbeats,
        f.onset,
        buffer.sampleRate,
        512
    );

    return f;
}