import 'dart:math';

import 'package:confetti/confetti.dart';
import 'package:eigo/pages/result_page.dart';
import 'package:eigo/utils/websocket_manager.dart';
import 'package:flutter/material.dart';

class AnswerPage extends StatefulWidget {
  final String word;
  final String username;
  final int round;

  const AnswerPage({super.key, required this.word, required this.username, required this.round});

  @override
  State<StatefulWidget> createState() => _AnswerPageState();
}

class _AnswerPageState extends State<AnswerPage> {
  final _websocket = WebSocketService();
  final _formKey = GlobalKey<FormState>();
  final TextEditingController _textEditController = TextEditingController();
  final _confettiController = ConfettiController(duration: Duration(days: 1));
  final List<String> _statusMessages = ["回答する", "待機中"];
  final List<String> _resultMessages = ["回答待ち", "判定中", "成功！", "失敗"];
  int rank = 1;
  bool _waiting = false;
  bool? _result;
  String _resultMessage = "回答まち";
  String _status = "回答する";

  void _closeKeyboard() {
    final currentScope = FocusScope.of(context);
    if (!currentScope.hasPrimaryFocus && currentScope.hasFocus) {
      currentScope.unfocus();
    }
  }

  Widget _submitButton() {
    return Expanded(
      child: TextButton(
        style: TextButton.styleFrom(
          padding: const EdgeInsets.symmetric(vertical: 12, horizontal: 24),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(10),
          ),
          side: const BorderSide(
            color: Colors.black26,
            width: 1,
          ),
          foregroundColor: Colors.black,
          textStyle: TextStyle(
            fontSize: 20,
            fontWeight: FontWeight.w400,
          ),
        ),
        onPressed: () async {
          if (_waiting) {
            return;
          }

          if (!(_formKey.currentState?.validate() ?? false)) {
            return;
          }

          _closeKeyboard();
          setState(() {
            _waiting = true;
            _resultMessage = _resultMessages[1];

            _status = _statusMessages[1];
          });

          await Future.delayed(Duration(seconds: 1));
          setState(() {
            _result = Random().nextInt(2) == 0;
            if (_result ?? false) _confettiController.play();
            _resultMessage = _result ?? false ? _resultMessages[2]: _resultMessages[3];
          });
          await Future.delayed(Duration(seconds: 3));

          setState(() {
            _status = _statusMessages[0];

            _textEditController.text = "";
            _confettiController.stop();

            _result = null;
            _resultMessage = _resultMessages[0];
            _waiting = false;
          });

          if (widget.round >= 4) {
            Navigator.pushReplacement(
              context,
              MaterialPageRoute(builder: (context) => ResultPage())
            );
          } else {
            Navigator.pushReplacement(
              context,
              MaterialPageRoute(builder: (context) => AnswerPage(word: widget.word, username: widget.username, round: widget.round + 1))
            );
          }
        },
        child: Text(
          _status,
          style: TextStyle(
            color: _waiting ? Colors.black26 : Colors.black
          ),
        ),
      ),
    );
  }

  Widget _resultButton() {
    Color backgroundColor = _result == null ? Colors.grey :
          _result ?? false ? Colors.greenAccent : Colors.redAccent;
    return Expanded(
      child: Container(
        height: 50,
        decoration: BoxDecoration(
          color: backgroundColor,
          borderRadius: BorderRadius.all(Radius.circular(5)),
          border: Border.all(color: backgroundColor),
        ),
        child: Center(
          child: Text(
            _resultMessage,
            textAlign: TextAlign.center,
            style: TextStyle(
              fontSize: 20,
              color: Colors.white
            ),
          ),
        ),
      ),
    );
  }

  @override
  void dispose() {
    _confettiController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return PopScope(
      canPop: false,
      child: GestureDetector(
        onTap: _closeKeyboard,
        child: Scaffold(
          backgroundColor: Colors.white,
          body: Stack(
            children: [
              Container(
                margin: EdgeInsets.symmetric(horizontal: 20),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const SizedBox(height: 150),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.end,
                      children: [
                        Text(
                          "ラウンド: ${widget.round + 1}/5",
                          style: TextStyle(
                              fontSize: 20
                          ),
                        )
                      ],
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.end,
                      children: [
                        Icon(
                          Icons.emoji_events,
                          size: 40,
                          color: rank == 1 ? Colors.yellow :
                            rank == 2 ? Color.fromARGB(255, 200, 200, 200) :
                            rank == 3 ? Colors.brown : Colors.grey,
                        ),
                        Text("$rank位"),
                        const SizedBox(width: 10),
                        CircleAvatar(
                          radius: 24,
                          child: Icon(Icons.person, size: 40, color: Colors.white),
                        ),
                        const SizedBox(width: 10),
                        Text(
                          widget.username,
                          style: TextStyle(
                              fontSize: 20
                          ),
                        )
                      ],
                    ),
                    Text(
                      "今回のお題: ${widget.word}",
                      style: TextStyle(
                          fontSize: 18
                      ),
                    ),
                    Form(
                        key: _formKey,
                        child: TextFormField(
                          controller: _textEditController,
                          minLines: 1,
                          maxLines: 10,
                          validator: (text) {
                            if (text == null || text.isEmpty) {
                              return '必須項目です';
                            }
                            return null;
                          },
                          readOnly: _waiting,
                          decoration: InputDecoration(
                              border: OutlineInputBorder()
                          ),
                        )
                    ),
                    const SizedBox(height: 40),
                    Row(
                      children: [
                        _resultButton(),
                        const SizedBox(width: 10),
                        _submitButton()
                      ],
                    ),
                  ],
                ),
              ),
              Align(
                alignment: Alignment.bottomCenter,
                child: ConfettiWidget(
                  confettiController: _confettiController,
                  particleDrag: 0.02,
                  blastDirection: -pi / 2,
                  emissionFrequency: 0.1,
                  numberOfParticles: 20,
                  maxBlastForce: 90,
                  minBlastForce: 30,
                  gravity: 0.6,
                  colors: const [Colors.red, Colors.blue, Colors.green, Colors.orange],
                ),
              ),
            ],
          )
        ),
      )
    );
  }
}