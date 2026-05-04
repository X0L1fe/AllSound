#pragma once
#include "dsp/audio_features.h"
#include "dsp/audio_buffer.h"


class FeatureExtractor {
public:
    static AudioFeatures extract(const AudioBuffer& buffer);
};