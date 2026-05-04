#include <cmath>
#include <vector>
using namespace std;

float estimate_phase(
    const vector<int>& peaks,
    float bpm,
    int sampleRate,
    int hop
);