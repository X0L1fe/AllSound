#include "downbeat.h"
#include <cmath>
using namespace std;
// Пример структуры нейросетевой модели (ONNX/Libtorch)
// Real implementation would load модель и выполнять инференс.

// Детектирование даунбитов через ML-модель

// --- ВСПОМОГАТЕЛЬНАЯ ---
static float get_onset_value(const vector<float>& onset, int idx) {
    if (idx < 0 || idx >= (int)onset.size()) return 0;
    return onset[idx];
}

// --- DOWNBEAT DETECTION ---
vector<float> detect_downbeats(
    const vector<float>& beats,
    const vector<float>& onset,
    int sampleRate,
    int hop
) {
    vector<float> downbeats;

    if (beats.size() < 4) return downbeats;

    // перевод beats → frame index
    vector<int> beatFrames;
    for (float t : beats) {
        int frame = (int)(t * sampleRate / hop);
        beatFrames.push_back(frame);
    }

    // ищем фазу такта (0..3)
    float bestScore = 0;
    int bestOffset = 0;

    for (int offset = 0; offset < 4; offset++) {

        float score = 0;

        for (size_t i = offset; i < beatFrames.size(); i += 4) {
            score += get_onset_value(onset, beatFrames[i]);
        }

        if (score > bestScore) {
            bestScore = score;
            bestOffset = offset;
        }
    }

    // собираем downbeats
    for (size_t i = bestOffset; i < beats.size(); i += 4) {
        downbeats.push_back(beats[i]);
    }

    return downbeats;
}

vector<int> detect_bars(const vector<float>& downbeats) {
    vector<int> bars;
    for (size_t i = 0; i < downbeats.size(); ++i) {
        bars.push_back((int)i); // например, номер такта = порядковый индекс
    }
    return bars;
}

// Фрагмент функции распознавания фраз (простой: каждые 4 такта)
vector<int> detect_phrases(
    const vector<float>& downbeats,
    const vector<float>& onset,
    int sampleRate,
    int hop
) {
    vector<int> phrases;

    if (downbeats.size() < 8) return phrases;

    // перевод в frame
    vector<int> dbFrames;
    for (float t : downbeats) {
        dbFrames.push_back((int)(t * sampleRate / hop));
    }

    // считаем энергию по барам
    vector<float> barEnergy;

    for (size_t i = 0; i + 1 < dbFrames.size(); i++) {
        float sum = 0;

        for (int f = dbFrames[i]; f < dbFrames[i+1]; f++) {
            if (f >= 0 && f < onset.size()) {
                sum += onset[f];
            }
        }

        barEnergy.push_back(sum);
    }

    // ищем пики изменения энергии
    vector<int> phraseIndex(barEnergy.size());

    int currentPhrase = 0;
    phraseIndex[0] = 0;

    for (size_t i = 1; i < barEnergy.size(); i++) {

        float diff = fabs(barEnergy[i] - barEnergy[i-1]);

        // ключевая эвристика
        if (diff > 0.3f) {  // можно тюнить
            currentPhrase++;
        }

        phraseIndex[i] = currentPhrase;
    }

    return phraseIndex;
}
