#pragma once
#include <string>
#include "audio_buffer.h"

class AudioDecoder {
public:
    static AudioBuffer decode(const std::string& path);
};