#include <iostream>
#include "core/feature_extractor.h"
#include "../decoder/audio_decoder.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

extern "C" {

float get_bpm(const char* file_path) {
    cout << "[AudioEngine] BPM: " << file_path << "\n";

    AudioBuffer buffer = AudioDecoder::decode(file_path);
    AudioFeatures features = FeatureExtractor::extract(buffer);

    return features.bpm;
}

float get_energy(const char* file_path) {
    cout << "[AudioEngine] Energy: " << file_path << "\n";

    AudioBuffer buffer = AudioDecoder::decode(file_path);
    AudioFeatures features = FeatureExtractor::extract(buffer);

    return features.energy;
}

int get_period(const char* file_path) {
    AudioBuffer buffer = AudioDecoder::decode(file_path);
    AudioFeatures features = FeatureExtractor::extract(buffer);

    return features.period;
}

int get_beats(const char* file_path, float* out_beats, int max_beats) {
    AudioBuffer buffer = AudioDecoder::decode(file_path);
    AudioFeatures features = FeatureExtractor::extract(buffer);

    int count = features.beats.size();
    if (count > max_beats) count = max_beats;

    for (int i = 0; i < count; i++) {
        out_beats[i] = features.beats[i];
    }

    return count;
}

// **Новый API**: get_features в JSON
// out_json_buffer - указатель на буфер для JSON, max_len - размер буфера
// Возвращает фактическую длину JSON (или -1 при ошибке)
int get_features(const char* file_path, char* out_json_buffer, int max_len) {
    AudioBuffer buffer = AudioDecoder::decode(file_path);
    AudioFeatures f = FeatureExtractor::extract(buffer);

    // Составляем JSON
    json J;
    J["bpm"] = f.bpm;
    J["energy"] = f.energy;
    J["period"] = f.period;
    J["beats"] = f.beats;
    J["downbeats"] = f.downbeats;
    J["downbeat_confidence"] = f.downbeat_confidence;
    J["bars"] = f.bar_index;
    J["phrases"] = f.phrase_index;
    // Можно добавить размер onset: J["onset_length"] = f.onset.size();

    string json_str = J.dump();
    if ((int)json_str.size() >= max_len) return -1;
    memcpy(out_json_buffer, json_str.c_str(), json_str.size()+1);
    return (int)json_str.size();
}

}