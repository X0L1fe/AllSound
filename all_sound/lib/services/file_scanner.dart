import 'dart:io';
import 'package:audio_metadata_reader/audio_metadata_reader.dart' as amr;
import 'package:permission_handler/permission_handler.dart';

class FileScanner {
  // Сканирование выбранных папок (список путей)
  static Future<List<Map<String, dynamic>>> scanDefaultFolders() async {
    List<Map<String, dynamic>> tracks = [];

    if (!await _requestPermission()) {
      print("Нет доступа к файлам");
      return tracks;
    }

    // Список стандартных папок с музыкой на Android
    List<String> musicFolders = [
      '/storage/emulated/0/Music',
      '/storage/emulated/0/Download',
      '/storage/emulated/0/Media/Music',
      '/storage/emulated/0/Audio',
    ];

    for (String folder in musicFolders) {
      Directory dir = Directory(folder);
      if (await dir.exists()) {
        await _scanDirectory(dir, tracks);
      }
    }

    return tracks;
  }

  static Future<void> _scanDirectory(
      Directory dir, List<Map<String, dynamic>> tracks) async {
    try {
      await for (FileSystemEntity entity in dir.list(recursive: true)) {
        if (entity is! File) continue;
        String path = entity.path;
        if (!_isAudioFile(path)) continue;

        try {
          final file = File(path);
          final metadata = await amr.readMetadata(file);
          final duration = metadata.duration?.inSeconds.toDouble() ?? 0.0;
          if (duration < 10) continue;

          tracks.add({
            "track_id": path,
            "path": path,
            "title": metadata.title ?? _fileName(path),
            "artist": metadata.artist ?? "Unknown",
            "album": metadata.album ?? "",
            "duration": duration,
          });
        } catch (e) {
          print("Ошибка чтения $path: $e");
        }
      }
    } catch (e) {
      print("Ошибка сканирования папки ${dir.path}: $e");
    }
  }

  static Future<bool> _requestPermission() async {
    // Запрашиваем READ_EXTERNAL_STORAGE (или для Android 13+ - READ_MEDIA_AUDIO)
    Permission permission;
    // Проверяем версию Android (упрощённо – всегда audio)
    if (await _isAndroid13OrNewer()) {
      permission = Permission.audio;
    } else {
      permission = Permission.storage;
    }
    var status = await permission.status;
    if (status.isDenied) {
      status = await permission.request();
    }
    return status.isGranted;
  }

  static Future<bool> _isAndroid13OrNewer() async {
    // реальную проверку версии можно сделать через device_info_plus
    // пока оставим true, чтобы запрашивался Permission.audio (он не вредит)
    return true;
  }

  static bool _isAudioFile(String path) {
    final ext = path.split('.').last.toLowerCase();
    return ["mp3", "flac", "wav", "aac", "ogg", "m4a", "alac"].contains(ext);
  }

  static String _fileName(String path) => path.split('/').last;
}
