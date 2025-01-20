import 'dart:async';
import 'dart:developer';

import 'package:flutter/material.dart';

import 'package:noko_clipboard/noko_clipboard.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final _nokoClipboardPlugin = NokoClipboard();
  late StreamSubscription<String?> _subs;

  @override
  void initState() {
    super.initState();
    initializeClipWatcher();
  }

  @override
  void dispose() {
    _subs.cancel();
    _nokoClipboardPlugin.dispose();
    super.dispose();
  }

  void initializeClipWatcher() {
    _subs = _nokoClipboardPlugin.clipDataStream.listen(
      (event) {
        log('Clip data: $event');
      },
    );
    _nokoClipboardPlugin.initialize();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: const Center(
          child: Text('Test clip watcher...\n'),
        ),
      ),
    );
  }
}
