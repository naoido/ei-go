import 'package:web_socket_channel/web_socket_channel.dart';

class WebSocketService {
  static final WebSocketService _instance = WebSocketService._internal();
  late final WebSocketChannel _channel;
  final String _wsUrl = "wss://hackz.naoido.com/ws";

  factory WebSocketService() => _instance;

  WebSocketService._internal() {
    _channel = WebSocketChannel.connect(Uri.parse(_wsUrl));
  }

  void sendMessage(String message) async {
    await _channel.ready;
    if (_channel.closeCode == null) {
      _channel.sink.add(message);
      print("送信: $message");
    } else {
      print("WebSocket が閉じています");
    }
  }

  Stream get messages => _channel.stream;

  void closeConnection() {
    _channel.sink.close();
  }
}
