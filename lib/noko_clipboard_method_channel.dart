import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'noko_clipboard_platform_interface.dart';

/// An implementation of [NokoClipboardPlatform] that uses method channels.
class MethodChannelNokoClipboard extends NokoClipboardPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('noko_clipboard');

  /// Event channel that can be listen to get clip data changed event.
  final eventChannel =
      const EventChannel('noko_clipboard/clipdata_event_channel');

  @override
  Future<void> initialize() async {
    await methodChannel.invokeMethod('initialize');
  }

  @override
  Future<void> dispose() async {
    await methodChannel.invokeMethod('dispose');
  }

  @override
  // TODO: implement clipDataStream
  Stream<String?> get clipDataStream =>
      eventChannel.receiveBroadcastStream().cast();
}
