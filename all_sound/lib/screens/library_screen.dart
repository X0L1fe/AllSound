import 'package:flutter/material.dart';
import 'package:all_sound/services/local_storage.dart';
import 'package:all_sound/services/api_service.dart';
import 'package:all_sound/screens/scan_screen.dart';

class LibraryScreen extends StatefulWidget {
  @override
  _LibraryScreenState createState() => _LibraryScreenState();
}

class _LibraryScreenState extends State<LibraryScreen> {
  List<Map<String, dynamic>> _tracks = [];

  @override
  void initState() {
    super.initState();
    _loadTracks();
  }

  void _loadTracks() {
  setState(() {
      _tracks = List<Map<String, dynamic>>.from(LocalStorage.getAllTracks());
    });
  }

  Future<void> _refreshFromServer() async {
    final result = await ApiService.exportAll();
    if (result != null) {
      for (var tr in result) {
        await LocalStorage.addTrack(tr);
      }
      _loadTracks(); // обновляем список
    }
  }

  void _goToScanScreen() {
    Navigator.push(
      context,
      MaterialPageRoute(builder: (_) => ScanScreen()),
    ).then((_) => _loadTracks()); // после сканирования обновляем библиотеку
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Библиотека"),
        actions: [
          // Кнопка перехода на сканер
          IconButton(
            icon: const Icon(Icons.queue_music),
            onPressed: _goToScanScreen,
            tooltip: 'Сканировать музыку',
          ),
          // Кнопка выхода
          Padding(
            padding: const EdgeInsets.only(right: 8.0),
            child: ElevatedButton(
              onPressed: () async {
                await LocalStorage.clearUser();
                if (context.mounted) {
                  Navigator.pushNamedAndRemoveUntil(
                    context,
                    '/login',
                    (route) => false,
                  );
                }
              },
              child: const Text("Выйти"),
            ),
          ),
        ],
      ),
      body: _tracks.isEmpty
          ? const Center(child: Text("Нет треков. Нажмите на иконку 🎵 вверху, чтобы добавить музыку"))
          : ListView.builder(
              itemCount: _tracks.length,
              itemBuilder: (ctx, i) {
                final track = _tracks[i];
                return ListTile(
                  title: Text(track['title'] ?? 'Без названия'),
                  subtitle: Text(track['artist'] ?? 'Неизвестный исполнитель'),
                  onTap: () {
                    Navigator.pushNamed(context, '/player', arguments: track);
                  },
                  trailing: IconButton(
                    icon: const Icon(Icons.favorite_border),
                    onPressed: () {
                      // TODO: добавить в избранное
                    },
                  ),
                );
              },
            ),
      floatingActionButton: FloatingActionButton(
        child: const Icon(Icons.refresh),
        onPressed: _refreshFromServer,
        tooltip: 'Синхронизировать с сервером',
      ),
    );
  }
}