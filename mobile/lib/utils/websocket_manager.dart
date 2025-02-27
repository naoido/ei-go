import 'dart:async';
import 'package:web_socket_channel/web_socket_channel.dart';

class WebSocketService {
  static final WebSocketService _instance = WebSocketService._internal();
  late WebSocketChannel _channel;
  final String _wsUrl = "wss://hackz.naoido.com/ws?roomId=";
  final StreamController _messageController = StreamController.broadcast();

  factory WebSocketService() => _instance;

  WebSocketService._internal();

  Future<void> initialize(String roomId) async {
    _channel = WebSocketChannel.connect(Uri.parse(_wsUrl + roomId));
    _channel.stream.listen(
          (message) {
        _messageController.add(message);
      },
      onDone: () {
        _messageController.close();
      },
      onError: (error) {
        _messageController.addError(error);
      },
    );
  }

  void sendMessage(String message) {
    if (_channel.closeCode == null) {
      _channel.sink.add(message);
      print("送信: $message");
    } else {
      print("WebSocket が閉じています");
    }
  }

  Stream get messages => _messageController.stream;

  void closeConnection() {
    _channel.sink.close();
    _messageController.close();
  }
}
