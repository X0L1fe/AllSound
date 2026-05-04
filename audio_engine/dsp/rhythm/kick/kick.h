#pragma once
#include "../../audio_buffer.h"
#include <vector>

std::vector<float> detect_kicks(const AudioBuffer& buffer);