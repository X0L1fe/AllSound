import 'package:flutter/material.dart';
import 'package:just_audio/just_audio.dart';

/// Экран плеера.
class PlayerScreen extends StatefulWidget {
  @override
  _PlayerScreenState createState() => _PlayerScreenState();
}

class _PlayerScreenState extends State<PlayerScreen> {
  late AudioPlayer _audioPlayer;
  bool _playing = false;

  @override
  void initState() {
    super.initState();
    _audioPlayer = AudioPlayer();
    final track = ModalRoute.of(context)!.settings.arguments as Map<String, dynamic>;
    _audioPlayer.setFilePath(track['path']);
  }

  @override
  void dispose() {
    _audioPlayer.dispose();
    super.dispose();
  }

  void _togglePlay() {
    if (_playing) {
      _audioPlayer.pause();
    } else {
      _audioPlayer.play();
    }
    setState(() {
      _playing = !_playing;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("Плеер")),
      body: Center(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            IconButton(
              iconSize: 64,
              icon: Icon(_playing ? Icons.pause_circle : Icons.play_circle),
              onPressed: _togglePlay,
            ),
            Text("Сейчас играет")
          ],
        ),
      ),
    );
  }
}
