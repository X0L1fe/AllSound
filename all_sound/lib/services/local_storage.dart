import 'dart:convert';
import 'dart:io';
import 'package:path_provider/path_provider.dart';
import '../models/user_model.dart';

class LocalStorage {
  static late File _libraryFile;
  static late File _userFile;

  static List<Map<String, dynamic>> tracks = [];

  // ================= INIT =================
  static Future<void> init() async {
    final dir = await getApplicationDocumentsDirectory();

    _libraryFile = File('${dir.path}/library.json');
    _userFile = File('${dir.path}/user.json');

    // library
    if (!(await _libraryFile.exists())) {
      await _libraryFile.create(recursive: true);
      await _libraryFile.writeAsString(jsonEncode({"tracks": []}));
    }

    final libContent = await _libraryFile.readAsString();
    final libData = jsonDecode(libContent);
    tracks = (libData['tracks'] as List?)?.cast<Map<String, dynamic>>() ?? [];
    // user
    if (!(await _userFile.exists())) {
      await _userFile.create(recursive: true);
      await _userFile.writeAsString(jsonEncode({}));
    }
  }

  // ================= USER =================
  static Future<void> saveUser(User user) async {
    await _userFile.writeAsString(jsonEncode(user.toJson()));
  }

  static Future<User?> getUser() async {
    final content = await _userFile.readAsString();
    if (content.isEmpty) return null;

    final json = jsonDecode(content);
    if (json.isEmpty) return null;

    return User.fromJson(json);
  }

  static Future<void> clearUser() async {
    await _userFile.writeAsString(jsonEncode({}));
  }

  // ================= TRACKS =================
  static Future<void> saveTracks() async {
    final data = {"tracks": tracks};
    await _libraryFile.writeAsString(jsonEncode(data));
  }

  static Future<void> addTrack(Map<String, dynamic> track) async {
    tracks.removeWhere((t) => t['track_id'] == track['track_id']);
    tracks.add(track);
    await saveTracks();
  }

  static List<Map<String, dynamic>> getAllTracks() {
    return tracks;
  }
}