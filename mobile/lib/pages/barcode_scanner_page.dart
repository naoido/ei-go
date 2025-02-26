import 'dart:async';

import 'package:eigo/pages/standby_page.dart';
import 'package:eigo/utils/websocket_manager.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:mobile_scanner/mobile_scanner.dart';

class BarcodeScannerPage extends StatefulWidget {
  const BarcodeScannerPage({super.key});

  @override
  State<BarcodeScannerPage> createState() => _BarcodeScannerPageState();
}

class _BarcodeScannerPageState extends State<BarcodeScannerPage> {
  final MobileScannerController controller = MobileScannerController(
    torchEnabled: false,
  );

  double _zoomFactor = 0.0;
  bool _isProcessing = false;

  Widget _buildZoomScaleSlider() {
    return ValueListenableBuilder(
      valueListenable: controller,
      builder: (context, state, child) {
        if (!state.isInitialized || !state.isRunning) {
          return const SizedBox.shrink();
        }

        final TextStyle labelStyle = Theme.of(context)
            .textTheme
            .headlineMedium!
            .copyWith(color: Colors.white);

        return Padding(
          padding: const EdgeInsets.symmetric(horizontal: 8.0),
          child: Row(
            children: [
              Text(
                '0%',
                overflow: TextOverflow.fade,
                style: labelStyle,
              ),
              Expanded(
                child: Slider(
                  value: _zoomFactor,
                  onChanged: (value) {
                    setState(() {
                      _zoomFactor = value;
                      controller.setZoomScale(value);
                    });
                  },
                ),
              ),
              Text(
                '100%',
                overflow: TextOverflow.fade,
                style: labelStyle,
              ),
            ],
          ),
        );
      },
    );
  }

  void _onBarcodeDetected(BarcodeCapture capture) async {
    if (_isProcessing || capture.barcodes.isEmpty) return;

    setState(() {
      _isProcessing = true;
    });

    final String barcodeValue = capture.barcodes.first.rawValue ?? 'Unknown';

    await WebSocketService().initialize(barcodeValue);
    Navigator.pushReplacement(
      context,
      MaterialPageRoute(
        builder: (context) => StandbyPage(barcodeValue: barcodeValue),
      ),
    ).then((_) {
      _isProcessing = false;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.white,
      body: Stack(
        children: [
          Stack(
            children: [
              MobileScanner(
                controller: controller,
                fit: BoxFit.contain,
                onDetect: _onBarcodeDetected,
              ),
              Align(
                alignment: Alignment.bottomCenter,
                child: Container(
                  alignment: Alignment.bottomCenter,
                  height: 100,
                  color: Colors.white,
                  child: Column(
                    children: [
                      if (!kIsWeb) _buildZoomScaleSlider(),
                    ],
                  ),
                ),
              ),
            ],
          ),
          if (_isProcessing) Container(
            color: Color.fromARGB(128, 0, 0, 0),
            child: const Center(
              child: CircularProgressIndicator(
                color: Colors.blue,
              )
            ),
          ),
        ],
      ),
    );
  }

  @override
  Future<void> dispose() async {
    super.dispose();
    await controller.dispose();
  }
}