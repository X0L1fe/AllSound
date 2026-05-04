```
audio_engine/
│
├── core/
│   ├── feature_extractor.cpp
│   ├── feature_extractor.h
│   └── core.cpp           # Основные C-API функции (get_bpm, get_features и др.)
│
├── dsp/
│   ├── beat/
│   │   ├── beat.cpp
│   │   ├── beat.h
│   │
│   ├── onset/
│   │   ├── onset_envelope.cpp
│   │   ├── onset.h
│   │
│   ├── tempo/
│   │   ├── bpm.cpp
│   │   ├── bpm.h
│   │
│   ├── structure/         # новый модуль для тактовой структуры
│   │   ├── downbeat.cpp
│   │   ├── downbeat.h
│   │   └── phrase.cpp      # (опционально, фразовая детекция)
│
├── decoder/
│   ├── audio_decoder.cpp
│   ├── audio_decoder.h
│
├── audio_buffer.h
├── energy.h
├── silence.h
│
├── CMakeLists.txt
└── main.cpp

```