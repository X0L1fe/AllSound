import 'dart:convert';
import 'package:http/http.dart' as http;

/// Сервис взаимодействия с backend API.
class ApiService {
  static const String baseUrl = "http://10.0.2.2:8000";

  /// Отправить трек на анализ.
  static Future<bool> analyzeTrack(Map<String, dynamic> trackData) async {
  try {
    final url = Uri.parse("$baseUrl/analyze");

    final response = await http.post(
      url,
      headers: {'Content-Type': 'application/json'},
      body: jsonEncode(trackData),
    );

    print("ANALYZE STATUS: ${response.statusCode}");
    print("BODY: ${response.body}");

    return response.statusCode == 200;
  } catch (e) {
    print("API ERROR: $e");
    return false;
  }
}
  /// Получить статус очереди.
  static Future<Map<String, dynamic>?> getStatus() async {
    final url = Uri.parse("$baseUrl/status");
    final response = await http.get(url);
    if (response.statusCode == 200) {
      return jsonDecode(response.body);
    }
    return null;
  }

  /// Экспортировать все данные (tracks).
  static Future<List<dynamic>?> exportAll() async {
    final url = Uri.parse("$baseUrl/export");
    final response = await http.get(url);
    if (response.statusCode == 200) {
      final data = jsonDecode(response.body);
      return data['tracks'];
    }
    return null;
  }
}
