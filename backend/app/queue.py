import threading
import queue
import logging
from app.analyzer import analyze_track

class AnalysisQueue:
    """
    Класс для управления очередью анализа треков.
    Использует стандартную очередь queue.Queue с фоновым воркером.
    """
    def __init__(self, max_workers: int = 5):
        self.queue = queue.Queue(maxsize=max_workers)
        self.logger = logging.getLogger("AnalysisQueue")
        self.logger.info("Запуск рабочего потока анализа.")
        self.worker = threading.Thread(target=self._worker_loop, daemon=True)
        self.worker.start()

    def enqueue(self, track_data: dict):
        """
        Положить задачу на анализ трека в очередь.
        """
        self.logger.debug(f"Добавление трека в очередь: {track_data.get('track_id')}")
        self.queue.put(track_data)

    def _worker_loop(self):
        """
        Бесконечный цикл: берём задачи из очереди и обрабатываем.
        """
        while True:
            try:
                track_data = self.queue.get()
                if track_data is None:
                    break  # выход по sentinel (если потребуется)
                self.logger.info(f"Начало анализа трека: {track_data.get('track_id')}")
                analyze_track(track_data)
                self.logger.info(f"Анализ завершён для трека: {track_data.get('track_id')}")
            except Exception as e:
                self.logger.error(f"Ошибка при анализе трека: {e}")
            finally:
                self.queue.task_done()

    def size(self):
        """Текущее число задач в очереди."""
        return self.queue.qsize()
