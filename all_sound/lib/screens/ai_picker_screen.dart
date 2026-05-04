import 'package:flutter/material.dart';
import 'package:all_sound/services/local_storage.dart';

/// Экран AI-подбора по настроению/жанру.
class AIPickerScreen extends StatefulWidget {
  @override
  _AIPickerScreenState createState() => _AIPickerScreenState();
}

class _AIPickerScreenState extends State<AIPickerScreen> {
  final _controller = TextEditingController();
  List<dynamic> _results = [];

  void _search() {
    final query = _controller.text.toLowerCase();
    final allTracks = LocalStorage.getAllTracks();
    // Простейшая фильтрация по mood/genre: вернуть если содержит query
    setState(() {
      _results = allTracks.where((tr) {
        final moods = List<String>.from(tr['analysis']['mood'] ?? []);
        return moods.any((m) => m.contains(query));
      }).toList();
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("AI Подбор по настроению")),
      body: Column(
        children: [
          Padding(
            padding: EdgeInsets.all(8.0),
            child: TextField(
              controller: _controller,
              decoration: InputDecoration(
                labelText: "Введите настроение или жанр",
                suffixIcon: IconButton(
                  icon: Icon(Icons.search),
                  onPressed: _search,
                ),
              ),
              onSubmitted: (_) => _search(),
            ),
          ),
          Expanded(
            child: ListView.builder(
              itemCount: _results.length,
              itemBuilder: (ctx, i) {
                final track = _results[i];
                return ListTile(
                  title: Text(track['title']),
                  subtitle: Text(track['artist'] ?? ""),
                  onTap: () {
                    Navigator.pushNamed(context, '/player', arguments: track);
                  },
                );
              },
            ),
          ),
        ],
      ),
    );
  }
}
