import 'package:flutter/material.dart';
import 'package:eigo/pages/barcode_scanner_page.dart';


void main() {
  runApp(
    const MaterialApp(
      title: 'EiGo!!',
      home: MyHome(),
    ),
  );
}

class MyHome extends StatelessWidget {
  const MyHome({super.key});

  Widget _buildItem(BuildContext context, String label, Widget page) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: Center(
        child: ElevatedButton(
          onPressed: () {
            Navigator.of(context).push(
              MaterialPageRoute(
                builder: (context) => page,
              ),
            );
          },
          child: Text(label),
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.center,
          children: [
            const Text('英GO!!', style: TextStyle(fontStyle: FontStyle.italic, fontSize: 40)),
            const SizedBox(height: 20),
            _buildItem(
              context,
              'ルームに参加する',
              const BarcodeScannerPage(),
            ),
          ],
        ),
      )
    );
  }
}