# noko_clipboard

A Flutter plugin to observe system clipboard changed. Currently only Linux platforms are supported.

## Getting Started

Create a plugin instance in dart side as below

```dart
final _nokoClipboardPlugin = NokoClipboard();
```

Before start observing the clipboard, you must register a callback to clip data stream ```clipDataStream```. Event data is text value (```String```).

```dart
_subscription = _nokoClipboardPlugin.clipDataStream.listen(
      (event) {
        ...your handler here
      },
    );
```

After that, call ```initialize``` to start observing the clipboard changes.

```dart
_nokoClipboardPlugin.initialize();
```

Good lucks!!
