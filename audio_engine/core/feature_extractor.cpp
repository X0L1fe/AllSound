#include "feature_extractor.h"
#include "dsp/energy.h"
#include "dsp/silence.h"

#include "dsp/onset/onset.h"
#include "dsp/tempo/bpm.h"
#include "dsp/beat/beat.h"
#include <audio_features.h>
#include "dsp/structure/downbeat.h"


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

    f.beats = build_beat_grid(
        f.bpm,
        phase,
        f.onset.size(),
        buffer.sampleRate,
        512
    );

    f.beats = detect_beats(buffer);
    f.downbeats = detect_downbeats(buffer);
    f.bar_index = detect_bars(f.downbeats);
    f.phrase_index = detect_phrases(f.bar_index);

    return f;
}