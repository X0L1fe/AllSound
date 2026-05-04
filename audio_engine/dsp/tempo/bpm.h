#include <vector>
#include <cmath>
#include <map>
using namespace std;
// Pipline который должен быть
// Audio → Envelope → Onset Strength → Autocorrelation → Peak → BPM
int autocorrelation(const vector<float>& onset) {

    int bestLag = 0;
    float bestCorr = 0;

    for (int lag = 20; lag < 200; lag++) {

        if (onset.size() <= lag) continue;

        float corr = 0;
        float norm = 0;

        size_t max_i = onset.size() - lag;

        for (size_t i = 0; i < max_i; i++) {
            corr += onset[i] * onset[i + lag];
            norm += onset[i] * onset[i];
        }

        if (norm > 0) corr /= norm;

        if (corr > bestCorr) {
            bestCorr = corr;
            bestLag = lag;
        }
    }

    return bestLag;
}

float detect_bpm(const vector<float>& onset, int sampleRate, int hop) {

    int minBPM = 60;
    int maxBPM = 180;

    int minLag = static_cast<int>((60.0f / maxBPM) * sampleRate / hop);
    int maxLag = static_cast<int>((60.0f / minBPM) * sampleRate / hop);

    float bestCorr = 0;
    int bestLag = 0;

    vector<pair<int, float>> peaks;
    float threshold = 0.3f;

    for (int lag = minLag; lag <= maxLag; lag++) {
        if (onset.size() <= lag) continue;

        float corr = 0;

        for (size_t i = 0; i < onset.size() - lag; i++) {
            corr += onset[i] * onset[i + lag];
        }

        if (corr > bestCorr) {
            bestCorr = corr;
            bestLag = lag;
        }

        if (corr > threshold) {
            peaks.push_back({lag, corr});
        }
    }

    if (bestLag == 0) return 0;

    if (peaks.empty()) {
        float bpm = 60.0f * sampleRate / (bestLag * hop);

        while (bpm < 80) bpm *= 2;
        while (bpm > 160) bpm /= 2;

        return bpm;
    }

    map<int, float> bpmVotes; // выбор более лучшего bpm чтобы исключить хрень

    for (auto& p : peaks) {
        int lag = p.first;
        float corr = p.second;

        float bpmCandidate = 60.0f * sampleRate / (lag * hop);

        // нормализация
        if (bpmCandidate < 80) bpmCandidate *= 2;
        if (bpmCandidate > 160) bpmCandidate /= 2;

        int rounded = (int)(bpmCandidate + 0.5f);
        bpmVotes[rounded] += corr;
    }

    // выбор лучшего
    float bestScore = 0;
    int bestBPM = 0;

    for (auto& kv : bpmVotes) {
        if (kv.second > bestScore) {
            bestScore = kv.second;
            bestBPM = kv.first;
        }
    }

    return (float)bestBPM;;
}