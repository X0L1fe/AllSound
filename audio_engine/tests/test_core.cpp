#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <cmath>

// Прототипы функций из core.cpp
extern "C" {
    float get_bpm(const char* file_path);
    float get_energy(const char* file_path);
    void crossfade(const char* file_a, const char* file_b, const char* output_file);
    void beat_sync(const char* file_a, const char* file_b, const char* output_file);
}

TEST_CASE("AudioEngine: get_bpm возвращает 120.0", "[audio]") {
    REQUIRE(get_bpm("dummy.mp3") == Approx(120.0f));
}

TEST_CASE("AudioEngine: get_energy возвращает 0.5", "[audio]") {
    REQUIRE(get_energy("dummy.mp3") == Approx(0.5f));
}

TEST_CASE("AudioEngine: crossfade и beat_sync (заглушки) не аварийны", "[audio]") {
    // Проверка того, что вызов функций не приводит к сбою
    crossfade("a.mp3", "b.mp3", "out.mp3");
    beat_sync("a.mp3", "b.mp3", "out.mp3");
    SUCCEED("Crossfade and beat_sync executed without crash");
}
