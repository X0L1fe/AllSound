import os
import json
import logging
from fastapi import FastAPI, HTTPException, BackgroundTasks
from fastapi.middleware.cors import CORSMiddleware
from app.models import TrackSchema, TrackWithAnalysis
from app.queue import AnalysisQueue

# Настройка логирования
logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s [%(levelname)s] %(name)s: %(message)s')
logger = logging.getLogger("API")

# Инициализация приложения
app = FastAPI(title="AI Music Player API")

# CORS (если требуется доступ с Flutter Desktop или Web)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

# Настройка очереди анализа
queue = AnalysisQueue(max_workers=5)

DATA_DIR = os.getenv("DATA_DIR", "data")

@app.post("/analyze")
async def analyze(track: TrackSchema, background_tasks: BackgroundTasks):
    """
    Эндпоинт для запуска анализа трека.
    Трек попадает в фоновую очередь.
    """
    try:
        track_dict = track.dict()
        background_tasks.add_task(queue.enqueue, track_dict)
        logger.info(f"POST /analyze: трек {track.track_id} поставлен в очередь.")
        return {"status": "queued", "track_id": track.track_id}
    except Exception as e:
        logger.error(f"Ошибка в /analyze: {e}")
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/status")
def status():
    """
    Возвращает статус очереди анализа:
    сколько задач ждёт и сколько обрабатывается.
    """
    queued = queue.size()
    # тут можно добавить более детальную информацию при необходимости
    return {"queued": queued, "processing": max(0, 5 - queued)}

@app.get("/export")
def export_data():
    """
    Экспорт всех проанализированных треков в JSON.
    Собирает файлы из DATA_DIR.
    """
    try:
        all_tracks = []
        if os.path.isdir(DATA_DIR):
            for fname in os.listdir(DATA_DIR):
                if fname.endswith(".json"):
                    with open(os.path.join(DATA_DIR, fname), 'r', encoding='utf-8') as f:
                        all_tracks.append(json.load(f))
        return {"tracks": all_tracks}
    except Exception as e:
        logger.error(f"Ошибка в /export: {e}")
        raise HTTPException(status_code=500, detail=str(e))
