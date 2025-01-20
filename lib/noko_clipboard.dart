import 'noko_clipboard_platform_interface.dart';

class NokoClipboard {
  Future<void> initialize() {
    return NokoClipboardPlatform.instance.initialize();
  }

  Future<void> dispose() {
    return NokoClipboardPlatform.instance.dispose();
  }

  Stream<String?> get clipDataStream =>
      NokoClipboardPlatform.instance.clipDataStream;
}
