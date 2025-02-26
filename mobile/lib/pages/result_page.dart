import 'package:flutter/material.dart';

class ResultPage extends StatefulWidget {
  const ResultPage({super.key});

  @override
  State<StatefulWidget> createState() => _ResultPage();
}

class _ResultPage extends State<ResultPage> {
  Map<String, int> users = {
    "naoido": 100,
    "thirdlf": 80,
    "user": 70,
    "うさぎさん1": 60,
    "うさぎさん2": 60,
    "うさぎさん3": 60,
    "うさぎさん4": 60,
    "うさぎさん5": 60,
    "うさぎさん6": 60,
    "うさぎさん7": 60,
    "うさぎさん8": 60,
    "うさぎさん9": 60,
    "うさぎさん1000000": 60,
  };

  Widget _rankWidget(int index, String username, int score) {
    int rank = index + 1;

    return Padding(
      padding: EdgeInsets.only(left: MediaQuery.of(context).size.width / 2 - 140),
      child: Column(
        children: [
          Row(
            crossAxisAlignment: CrossAxisAlignment.center,
            mainAxisAlignment: MainAxisAlignment.start,
            children: [
              Icon(
                Icons.emoji_events,
                size: 60,
                color: rank == 1 ? Colors.yellow :
                rank == 2 ? Color.fromARGB(255, 200, 200, 200) :
                rank == 3 ? Colors.brown : Colors.grey,
              ),
              Text(
                "$rank位",
                style: TextStyle(
                    fontSize: index < 3 ? 30 : 20
                ),
              ),
              const SizedBox(width: 10),
              SizedBox(
                width: 140,
                child: Text(
                  username,
                  style: TextStyle(
                      fontSize: index < 3 ? 30 : 20
                  ),
                  overflow: TextOverflow.ellipsis,
                ),
              )
            ],
          ),
          Text("スコア: $score")
        ],
      ),
    );
  }


  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(backgroundColor: Colors.white, scrolledUnderElevation: 0),
      backgroundColor: Colors.white,
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const Text(
              "今回の結果",
              style: TextStyle(
                fontSize: 40,
                fontStyle: FontStyle.italic
              ),
            ),
            Expanded(
              child: ListView(
                shrinkWrap: true,
                children: [
                  ...users.entries.toList().asMap().entries
                      .map((entry) => _rankWidget(entry.key, entry.value.key, entry.value.value))
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}