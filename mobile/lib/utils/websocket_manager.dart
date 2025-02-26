import 'dart:ffi';

import 'package:web_socket_channel/web_socket_channel.dart';

class WebSocketService {
  static final WebSocketService _instance = WebSocketService();
  late final WebSocketChannel _channel;
  final String _wsUrl = "wss://hackz.naoido.com/ws?roomId=";

  factory WebSocketService() => _instance;

  Future initialize(String roomId) async {
    _channel = WebSocketChannel.connect(Uri.parse(_wsUrl + roomId));
    await _channel.ready;
  }

  void sendMessage(String message) async {
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
