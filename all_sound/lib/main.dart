import 'package:flutter/material.dart';

import 'screens/login_screen.dart';
import 'screens/scan_screen.dart';
import 'screens/library_screen.dart';
import 'screens/player_screen.dart';
import 'screens/ai_picker_screen.dart';
import 'models/user_model.dart';
import 'services/local_storage.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  try {
    await LocalStorage.init();
    User? user = await LocalStorage.getUser();
    runApp(MyApp(initialRoute: user == null ? '/login' : '/library'));
  } catch (e, stack) {
    print('❌ Ошибка инициализации: $e');
    print(stack);
    // Всё равно запускаем приложение, но с экраном логина
    runApp(MyApp(initialRoute: '/login'));
  }
}

class MyApp extends StatelessWidget {
  final String initialRoute;
  const MyApp({super.key, required this.initialRoute});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'AllSound Player',
      debugShowCheckedModeBanner: false,

      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),

      initialRoute: initialRoute,

      routes: {
        '/login': (ctx) => LoginScreen(),
        '/scan': (ctx) => ScanScreen(),
        '/library': (ctx) => LibraryScreen(),
        '/player': (ctx) => PlayerScreen(),
        '/ai': (ctx) => AIPickerScreen(),
      },
    );
  }
}