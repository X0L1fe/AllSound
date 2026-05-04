import os
import pytest
from fastapi.testclient import TestClient
from app.main import app, DATA_DIR

client = TestClient(app)

def test_analyze_status_export(tmp_path):
    """
    Тесты эндпоинтов /analyze, /status и /export.
    """
    # Перенаправляем DATA_DIR во временную папку
    os.environ['DATA_DIR'] = str(tmp_path)
    # POST /analyze
    response = client.post("/analyze", json={
        "track_id": "test1",
        "path": "/path/to/fake.mp3",
        "title": "Test Song",
        "artist": "Artist",
        "album": "Album",
        "duration": 100.0
    })
    assert response.status_code == 200
    data = response.json()
    assert data["status"] == "queued" and data["track_id"] == "test1"

    # GET /status
    status_resp = client.get("/status")
    assert status_resp.status_code == 200
    status_data = status_resp.json()
    assert "queued" in status_data

    # Симулируем создание JSON вручную
    os.makedirs(tmp_path, exist_ok=True)
    sample = {
        "track_id": "test1",
        "path": "/path/to/fake.mp3",
        "title": "Test Song",
        "artist": "Artist",
        "album": "Album",
        "duration": 100.0,
        "analysis": {"bpm": 120.0, "key": "C", "energy": 0.5,
                     "mood": ["happy"], "genre": ["pop"],
                     "intro_duration": 5.0, "outro_duration": 5.0,
                     "vocal": True, "instrumental": False, "language": "ru"}
    }
    with open(tmp_path / "test1.json", "w", encoding="utf-8") as f:
        import json
        json.dump(sample, f)

    # GET /export
    export_resp = client.get("/export")
    assert export_resp.status_code == 200
    tracks = export_resp.json().get("tracks", [])
    assert any(t["track_id"] == "test1" for t in tracks)
