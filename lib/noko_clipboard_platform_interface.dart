import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'noko_clipboard_method_channel.dart';

abstract class NokoClipboardPlatform extends PlatformInterface {
  /// Constructs a NokoClipboardPlatform.
  NokoClipboardPlatform() : super(token: _token);

  static final Object _token = Object();

  static NokoClipboardPlatform _instance = MethodChannelNokoClipboard();

  /// The default instance of [NokoClipboardPlatform] to use.
  ///
  /// Defaults to [MethodChannelNokoClipboard].
  static NokoClipboardPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [NokoClipboardPlatform] when
  /// they register themselves.
  static set instance(NokoClipboardPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Stream<String?> get clipDataStream;

  Future<void> initialize() {
    throw UnimplementedError('initialize() has not been implemented.');
  }

  Future<void> dispose() {
    throw UnimplementedError('dispose() has not been implemented');
  }
}
