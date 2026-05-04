import 'package:flutter/material.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:file_picker/file_picker.dart';
import 'package:all_sound/services/file_scanner.dart';
import 'package:all_sound/services/api_service.dart';
import 'package:all_sound/services/local_storage.dart';

class ScanScreen extends StatefulWidget {
  @override
  _ScanScreenState createState() => _ScanScreenState();
}

class _ScanScreenState extends State<ScanScreen> {
  bool _scanning = false;
  bool _hasPermission = false;
  int _found = 0;
  List<Map<String, dynamic>> _detectedTracks = [];
  String _errorMessage = '';
  bool _stopRequested = false;
  List<String> _selectedFolders = [];

  @override
  void initState() {
    super.initState();
    _checkPermission();
  }

  Future<void> _checkPermission() async {
    Permission permission;
    if (await _isAndroid13OrNewer()) {
      permission = Permission.audio;
    } else {
      permission = Permission.storage;
    }

    final status = await permission.status;
    if (status.isDenied) {
      final result = await permission.request();
      setState(() {
        _hasPermission = result.isGranted;
        if (!_hasPermission) _errorMessage = 'Доступ к файлам запрещён';
      });
    } else {
      setState(() {
        _hasPermission = status.isGranted;
      });
    }
  }

  Future<bool> _isAndroid13OrNewer() async => true;

  Future<void> _pickFolder() async {
    String? selectedDirectory = await FilePicker.platform.getDirectoryPath();
    if (selectedDirectory != null && !_selectedFolders.contains(selectedDirectory)) {
      setState(() {
        _selectedFolders.add(selectedDirectory);
        _errorMessage = '';
      });
    }
  }

  void _removeFolder(String path) {
    setState(() {
      _selectedFolders.remove(path);
    });
  }

  Future<void> _startScan() async {
    if (!_hasPermission) {
      await _checkPermission();
      if (!_hasPermission) return;
    }
    setState(() {
      _scanning = true;
      _detectedTracks.clear();
      _found = 0;
      _errorMessage = '';
      _stopRequested = false;
    });

    try {
      List<Map<String, dynamic>> allTracks = await FileScanner.scanDefaultFolders();
      int total = allTracks.length;
      for (int i = 0; i < total; i++) {
        if (_stopRequested) break;
        final track = allTracks[i];
        ApiService.analyzeTrack(track);
        await LocalStorage.addTrack(track);
        setState(() {
          _found = i + 1;
          _detectedTracks.add(track);
        });
      }
    } catch (e) {
      setState(() {
        _errorMessage = 'Ошибка сканирования: $e';
      });
    } finally {
      setState(() {
        _scanning = false;
      });
    }
  }

  void _stopScan() {
    setState(() {
      _stopRequested = true;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Сканирование музыки"),
        actions: [
          if (_scanning)
            IconButton(
              icon: const Icon(Icons.stop),
              onPressed: _stopScan,
            ),
        ],
      ),
      body: Column(
        children: [
          // Сообщение об ошибке (всегда вверху)
          if (_errorMessage.isNotEmpty)
            Container(
              color: Colors.red.shade100,
              padding: const EdgeInsets.all(8),
              child: Text(_errorMessage, style: const TextStyle(color: Colors.red)),
            ),
          // Основное содержимое – меняется в зависимости от состояния
          if (!_hasPermission && !_scanning)
            const Expanded(
              child: Center(
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Text("Нет доступа к файлам"),
                    SizedBox(height: 16),
                    // Кнопку запроса доступа оставим, но она не в Expanded – вернём чуть позже
                  ],
                ),
              ),
            )
          else if (_scanning)
            Expanded(
              child: Column(
                children: [
                  LinearProgressIndicator(value: _found > 0 ? (_found / (_found + 20)) : null),
                  const SizedBox(height: 20),
                  Text("Найдено $_found треков"),
                  const SizedBox(height: 20),
                  Expanded(
                    child: ListView.builder(
                      itemCount: _detectedTracks.length,
                      itemBuilder: (ctx, i) {
                        final track = _detectedTracks[i];
                        return ListTile(
                          leading: const Icon(Icons.music_note),
                          title: Text(track['title'] ?? 'Без названия'),
                          subtitle: Text(track['artist'] ?? 'Неизвестный исполнитель'),
                        );
                      },
                    ),
                  ),
                ],
              ),
            )
          else
            // Режим выбора папок
            Expanded(
              child: Column(
                children: [
                  const SizedBox(height: 16),
                  ElevatedButton.icon(
                    icon: const Icon(Icons.folder_open),
                    label: const Text("Добавить папку с музыкой"),
                    onPressed: _pickFolder,
                  ),
                  const SizedBox(height: 16),
                  Expanded(
                    child: _selectedFolders.isEmpty
                        ? const Center(child: Text("Нет выбранных папок.\nНажмите кнопку, чтобы добавить."))
                        : ListView.builder(
                            itemCount: _selectedFolders.length,
                            itemBuilder: (ctx, i) {
                              final path = _selectedFolders[i];
                              return ListTile(
                                leading: const Icon(Icons.folder),
                                title: Text(path),
                                trailing: IconButton(
                                  icon: const Icon(Icons.remove_circle, color: Colors.red),
                                  onPressed: () => _removeFolder(path),
                                ),
                              );
                            },
                          ),
                  ),
                  const SizedBox(height: 16),
                  ElevatedButton(
                    onPressed: (_selectedFolders.isNotEmpty && !_scanning) ? _startScan : null,
                    child: const Text("Начать сканирование"),
                  ),
                  const SizedBox(height: 32),
                ],
              ),
            ),
        ],
      ),
    );
  }
}