#pragma once

#include <vector>
#include "audio_buffer.h"

std::vector<float> compute_onset_envelope(const AudioBuffer& buffer);