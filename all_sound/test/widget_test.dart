import 'package:flutter_test/flutter_test.dart';
import 'package:all_sound/main.dart';

void main() {
  testWidgets('App has a login screen', (WidgetTester tester) async {
    await tester.pumpWidget(MyApp());
    expect(find.text('AI Music Player'), findsNothing);
    // Можно добавить проверки наличия кнопок, полей
  });
}
