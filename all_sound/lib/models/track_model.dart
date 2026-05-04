import 'package:all_sound/models/feature_model.dart';
class Track {
  final String id;
  final String path;

  final String title;
  final String artist;

  final AudioFeatures features;

  Track({
    required this.id,
    required this.path,
    required this.title,
    required this.artist,
    required this.features,
  });

  Map<String, dynamic> toJson() => {
    "track_id": id,
    "path": path,
    "title": title,
    "artist": artist,
    "audio_features": features.toJson(),
  };
}