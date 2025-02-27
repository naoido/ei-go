import 'dart:convert';

import 'package:eigo/pages/answer_page.dart';
import 'package:eigo/utils/websocket_manager.dart';
import 'package:flutter/material.dart';

class StandbyPage extends StatefulWidget {
  final String barcodeValue;

  const StandbyPage({super.key, required this.barcodeValue});

  @override
  State<StandbyPage> createState() => _StandbyPageState();
}

class _StandbyPageState extends State<StandbyPage> {
  final WebSocketService _webSocket = WebSocketService();
  final TextEditingController _controller = TextEditingController();
  final _formKey = GlobalKey<FormState>();
  bool _isReady = false;

  void _closeKeyboard() {
    final currentScope = FocusScope.of(context);
    if (!currentScope.hasPrimaryFocus && currentScope.hasFocus) {
      currentScope.unfocus();
    }
  }

  @override
  Widget build(BuildContext context) {
    _webSocket.messages.listen((data) {
      Map<String, dynamic> message = jsonDecode(data);
      if (message["type"] == "question") {
        Navigator.pushReplacement(
          context,
          MaterialPageRoute(
              builder: (context) => AnswerPage(word: message["question"], username: _controller.text, round: 0, rank: 1)
          )
        );
      }
    });

    return PopScope(
      canPop: false,
      child: GestureDetector(
        onTap: _closeKeyboard,
        child: Scaffold(
          backgroundColor: Colors.white,
          body: Center(
            child: Container(
              margin: const EdgeInsets.symmetric(horizontal: 20),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.center,
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  Text(
                      widget.barcodeValue,
                      style: Theme.of(context).textTheme.headlineSmall
                  ),
                  Form(
                    key: _formKey,
                    child: TextFormField(
                      controller: _controller,
                      readOnly: _isReady,
                      maxLength: 10,
                      validator: (text) {
                        if (text == null || text.isEmpty) {
                          return '必須項目です';
                        }
                        return null;
                      },
                      decoration: InputDecoration(
                        labelText: "ユーザー名",
                        border: OutlineInputBorder(),
                      ),
                    ),
                  ),
                  const SizedBox(height: 10),
                  ElevatedButton(
                    onPressed: () {
                      if (!(_formKey.currentState?.validate() ?? false)) {
                        return;
                      }

                      _closeKeyboard();
                      _webSocket.sendMessage(jsonEncode({
                        "type": "update",
                        "name": _controller.text,
                        "isReady": true,
                      }));
                      setState(() {
                        _isReady = true;
                      });
                    },
                    style: ElevatedButton.styleFrom(
                      padding: const EdgeInsets.symmetric(horizontal: 30, vertical: 15),
                      backgroundColor: _isReady ? Colors.green : Colors.red,
                    ),
                    child: Text(
                      _isReady ? "準備完了" : "準備中",
                      style: const TextStyle(fontSize: 18, color: Colors.white),
                    ),
                  ),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
