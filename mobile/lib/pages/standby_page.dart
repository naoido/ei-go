import 'package:eigo/pages/answer_page.dart';
import 'package:flutter/material.dart';

class StandbyPage extends StatefulWidget {
  final String barcodeValue;

  const StandbyPage({super.key, required this.barcodeValue});

  @override
  State<StandbyPage> createState() => _StandbyPageState();
}

class _StandbyPageState extends State<StandbyPage> {
  final TextEditingController _controller = TextEditingController();
  bool _isReady = false;

  @override
  Widget build(BuildContext context) {
    return PopScope(
      canPop: true, // TODO: 遷移先の実装ができたらfalseにする
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
                TextField(
                  controller: _controller,
                  readOnly: _isReady,
                  maxLength: 10,
                  decoration: InputDecoration(
                    labelText: "ユーザー名(任意)",
                    border: OutlineInputBorder(),
                  ),
                ),
                const SizedBox(height: 10),
                ElevatedButton(
                  onPressed: () {
                    setState(() {
                      _isReady = !_isReady;
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
                TextButton(
                  onPressed: () {
                    Navigator.pushReplacement(
                      context,
                      MaterialPageRoute(
                        builder: (context) => AnswerPage()
                      )
                    );
                  },
                  child: const Text("テスト")
                )
              ],
            ),
          ),
        ),
      ),
    );
  }
}
