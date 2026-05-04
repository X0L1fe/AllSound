#pragma once
#include <vector>
using namespace std;

struct AudioBuffer {
    vector<float> samples;
    int sampleRate = 44100;
};