#include <iostream>
#include <filesystem>

using namespace std;
extern "C" {
    float get_bpm(const char* path);
    float get_energy(const char* path);
    int get_period(const char*);
    int get_beats(const char*, float*, int);
    int get_features(const char* file_path, char* out_json, int max_len);
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

    char json_buf[10000];
    int len = get_features(file.c_str(), json_buf, 10000);
    cout << len;
    if (len > 0) {
        cout << "Features JSON:\n" << json_buf << endl;
    } else {
        cout << "Error generating features JSON" << endl;
    }

    return 0;
}
