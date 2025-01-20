//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <noko_clipboard/noko_clipboard_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) noko_clipboard_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "NokoClipboardPlugin");
  noko_clipboard_plugin_register_with_registrar(noko_clipboard_registrar);
}
