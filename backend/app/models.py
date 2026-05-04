from pydantic import BaseModel, Field
from typing import List, Optional

class TrackSchema(BaseModel):
    """
    Модель запроса для постановки трека в очередь анализа.
    """
    track_id: str = Field(..., description="Уникальный ID трека")
    path: str = Field(..., description="Локальный путь к аудио файлу")
    title: Optional[str] = Field(None, description="Название трека")
    artist: Optional[str] = Field(None, description="Исполнитель")
    album: Optional[str] = Field(None, description="Альбом")
    duration: Optional[float] = Field(None, description="Длительность трека (сек.)")

class Analysis(BaseModel):
    """
    Результат AI-анализа трека.
    """
    bpm: float
    key: str
    energy: float
    mood: List[str]
    genre: List[str]
    intro_duration: float
    outro_duration: float
    vocal: bool
    instrumental: bool
    language: str

class TrackWithAnalysis(TrackSchema):
    """
    Трек с добавленным полем analysis.
    """
    analysis: Optional[Analysis] = Field(None, description="Результаты анализа")
