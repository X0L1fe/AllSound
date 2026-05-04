#include <iostream>
#include <filesystem>

using namespace std;
extern "C" {
    float get_bpm(const char* path);
    float get_energy(const char* path);
    int get_period(const char*);
    int get_beats(const char*, float*, int);
    #include <libavutil/avutil.h>
    #include <libavutil/log.h>
}



int main() {
    //cout << av_version_info() << endl;
    //cout << filesystem::current_path() << endl;

    //av_log_set_level(AV_LOG_DEBUG);
    string file = "test A.mp3";

    if (!filesystem::exists(file)) {
        cout << "File not found: " << file << endl;
        return 1;
    }

    float bpm = get_bpm(file.c_str());
    float energy = get_energy(file.c_str());
    int period = get_period(file.c_str());
    
    const int MAX_BEATS = 1000;
    float beats[MAX_BEATS];

    int beatCount = get_beats(file.c_str(), beats, MAX_BEATS);

    cout << "BPM: " << bpm << endl;
    cout << "Energy: " << energy << endl;
    cout << "Period: " << period << endl;

    cout << "Beats (" << beatCount << "):" << endl;

    for (int i = 0; i < beatCount && i < 20; i++) {
        cout << beats[i] << endl;
    }

    return 0;
}
