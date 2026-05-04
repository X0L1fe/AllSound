#pragma once
#include <vector>
using namespace std;

struct AudioFeatures {
    float bpm = 0;
    float energy = 0;
    int period = 0;       // отсчет между ударами

    vector<float> onset;
    vector<bool> silence;
    vector<float> beats;      // времена всех ударов (в секундах)
    vector<float> downbeats;  // времена ударов, начинающих такт
    vector<float> downbeat_confidence; // уверенность каждого downbeat
    vector<int> bar_index;    // номер такта для каждого downbeat
    vector<int> phrase_index; // номер фразы (каждые 16 тактов)
};
