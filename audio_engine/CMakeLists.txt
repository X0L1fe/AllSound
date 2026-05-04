cmake_minimum_required(VERSION 3.10)
project(AudioEngine CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(json
    GIT_REPOSITORY  https://github.com/nlohmann/json.git
    GIT_TAG         v3.11.3   # последняя стабильная версия
)
FetchContent_MakeAvailable(json)

file(GLOB_RECURSE DSP_SOURCES dsp/*.cpp)
file(GLOB_RECURSE DECODER_SOURCES decoder/*.cpp)
file(GLOB_RECURSE CORE_SOURCES core/*.cpp)


# === FFmpeg путь ===
set(FFMPEG_DIR "D:/ffmpeg")

include_directories(${FFMPEG_DIR}/include)
link_directories(${FFMPEG_DIR}/lib)

# === библиотека ===
add_library(audio_engine STATIC
    ${CORE_SOURCES}
    ${DSP_SOURCES}
    ${DECODER_SOURCES}
)

target_include_directories(audio_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/decoder
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp
)

target_link_libraries(audio_engine PUBLIC
    avformat
    avcodec
    avutil
    swresample
    nlohmann_json::nlohmann_json
)

# === тест ===
add_executable(test_app
    main.cpp
)

target_link_libraries(test_app
    audio_engine
    avformat
    avcodec
    avutil
    swresample
)