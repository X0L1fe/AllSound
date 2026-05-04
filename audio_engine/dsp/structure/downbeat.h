#pragma once
#include "../audio_buffer.h"
#include <vector>

std::vector<float> detect_beats(const AudioBuffer& buffer);
// Возвращает времена (в секундах) всех сильных битов

std::vector<float> detect_downbeats(const AudioBuffer& buffer);
// Возвращает времена даунбитов (на начало тактов)

std::vector<int> detect_bars(const std::vector<float>& downbeats);
// Преобразует each downbeat в индекс такта (0,1,2,...)

std::vector<int> detect_phrases(const std::vector<int>& bar_index);
// Определяет фразы (напр., каждые 4 или 8 тактов) – placeholder.
