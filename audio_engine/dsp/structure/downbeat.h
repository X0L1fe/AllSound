#pragma once
#include "../audio_buffer.h"
#include <vector>
using namespace std;

vector<float> detect_downbeats(
    const vector<float>& beats,
    const vector<float>& onset,
    int sampleRate,
    int hop
); // Возвращает времена даунбитов (на начало тактов)

vector<int> detect_bars(const vector<float>& downbeats);
// Преобразует each downbeat в индекс такта (0,1,2,...)

vector<int> detect_phrases(
    const vector<float>& downbeats,
    const vector<float>& onset,
    int sampleRate,
    int hop
);
// Определяет фразы (напр., каждые 4 или 8 тактов) – placeholder.
