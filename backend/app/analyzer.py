import os
import json
import librosa
import numpy as np
import logging
from datetime import datetime
from app.models import TrackSchema, Analysis
# Импортировать модули для MusicBrainz/LastFM, LLM (заглушки)
# import musicbrainzngs
# from openai_api import query_llm  # например, если используется OpenAI или локальная модель

logger = logging.getLogger("Analyzer")
DATA_DIR = os.getenv("DATA_DIR", "data")  # папка для хранения JSON результатов

def analyze_track(track_data: dict):
    """
    Анализ аудиотрека:
    - Загрузка файла через librosa
    - Вычисление BPM, тональности, энергии (RMS)
    - stub: определение вокала, инструментала, настроения (LLM)
    - Нормализация метаданных (MusicBrainz/LastFM - stub)
    - Сохранение результатов в JSON файл
    """
    track = TrackSchema(**track_data)
    file_path = track.path
    try:
        # Загрузка аудио (librosa) с сохранением оригинальной частоты дискретизации
        y, sr = librosa.load(file_path, sr=None, mono=True)
        duration = librosa.get_duration(y=y, sr=sr)
        logger.debug(f"Файл {file_path} загружен, sr={sr}, длительность={duration:.2f}s")

        # Расчёт темпа (BPM)
        onset_env = librosa.onset.onset_strength(y=y, sr=sr)
        tempo_list = librosa.beat.tempo(onset_envelope=onset_env, sr=sr)
        bpm = float(tempo_list.max()) if tempo_list.size > 0 else 0.0

        # Расчёт энергии (RMS)
        rms = librosa.feature.rms(y=y)
        energy = float(np.mean(rms))

        # stub: детекция тональности (например, C, Dm, etc. через pitch tracking)
        key = "C"  # placeholder

        # stub: определение вокала/инструментала (наличие вокала — пример)
        vocal = True
        instrumental = not vocal

        # stub: определение языка (на основе ID3 или LLM)
        language = "ru"

        # stub: LLM-анализ настроения/жанра по текствому описанию (если есть)
        mood = ["happy"]
        genre = ["pop"]

        # Начальная/финальная части (intro/outro) – placeholder
        intro_dur = 5.0
        outro_dur = 5.0

        analysis_result = Analysis(
            bpm=bpm,
            key=key,
            energy=energy,
            mood=mood,
            genre=genre,
            intro_duration=intro_dur,
            outro_duration=outro_dur,
            vocal=vocal,
            instrumental=instrumental,
            language=language
        )

        # Подготовка записи для сохранения
        record = {
            "track_id": track.track_id,
            "path": track.path,
            "title": track.title or os.path.basename(file_path),
            "artist": track.artist,
            "album": track.album,
            "duration": duration,
            "analysis": analysis_result.dict()
        }

        # Сохранение в JSON (файл с именем track_id.json)
        os.makedirs(DATA_DIR, exist_ok=True)
        out_path = os.path.join(DATA_DIR, f"{track.track_id}.json")
        with open(out_path, "w", encoding="utf-8") as f:
            json.dump(record, f, ensure_ascii=False, indent=2)

        logger.info(f"Результаты анализа сохранены: {out_path}")
    except Exception as e:
        logger.error(f"Не удалось проанализировать трек {track.track_id}: {e}")
