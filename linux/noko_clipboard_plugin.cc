#include "include/noko_clipboard/noko_clipboard_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>
#include <cstring>
#include <stdbool.h>

#include "noko_clipboard_plugin_private.h"
#include "clipboard_watcher.h"

#define NOKO_CLIPBOARD_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), noko_clipboard_plugin_get_type(), \
                              NokoClipboardPlugin))

struct _NokoClipboardPlugin
{
  GObject parent_instance;
};

G_DEFINE_TYPE(NokoClipboardPlugin, noko_clipboard_plugin, g_object_get_type())

static FlEventChannel *m_clipdata_event_channel = NULL;
static bool is_event_channel_connected = false;

/**
 * Handle method channel
 */

static void handle_clipdata_changed(const gchar *clipdata)
{
  if(m_clipdata_event_channel && is_event_channel_connected){
    FlValue *clipdata_val = fl_value_new_string(clipdata);
    fl_event_channel_send(m_clipdata_event_channel, clipdata_val, NULL, NULL);        
  }
}

static FlMethodResponse *handle_init_clip_watcher()
{
  clip_watcher_add_clipdata_changed_cb(handle_clipdata_changed);
  if (!clip_watcher_initialize())
  {
    return FL_METHOD_RESPONSE(
        fl_method_error_response_new("clip_watcher_init_failed", "Failed to initialize clip watcher", NULL));
  }

  return FL_METHOD_RESPONSE(fl_method_success_response_new(NULL));
}

static FlMethodResponse *handle_dispose_clip_watcher()
{
  if (!clip_watcher_dispose())
  {
    return FL_METHOD_RESPONSE(fl_method_error_response_new("clip_watcher_dispose_failed", "Failed to dispose clip watcher", NULL));
  }

  return FL_METHOD_RESPONSE(fl_method_success_response_new(NULL));
}

// Called when a method call is received from Flutter.
static void noko_clipboard_plugin_handle_method_call(
    NokoClipboardPlugin *self,
    FlMethodCall *method_call)
{
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar *method = fl_method_call_get_name(method_call);

  if (strcmp(method, "initialize") == 0)
  {
    response = handle_init_clip_watcher();
  }
  else if (strcmp(method, "dispose") == 0)
  {
    response = handle_dispose_clip_watcher();
  }
  else
  {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void noko_clipboard_plugin_dispose(GObject *object)
{
  G_OBJECT_CLASS(noko_clipboard_plugin_parent_class)->dispose(object);
  clip_watcher_dispose();
}

static void noko_clipboard_plugin_class_init(NokoClipboardPluginClass *klass)
{
  G_OBJECT_CLASS(klass)->dispose = noko_clipboard_plugin_dispose;
}

static void noko_clipboard_plugin_init(NokoClipboardPlugin *self) {}

static void method_call_cb(FlMethodChannel *channel, FlMethodCall *method_call,
                           gpointer user_data)
{
  NokoClipboardPlugin *plugin = NOKO_CLIPBOARD_PLUGIN(user_data);
  noko_clipboard_plugin_handle_method_call(plugin, method_call);
}

/**
 * Event channel handler
 */
static FlMethodErrorResponse *on_clipdata_event_chan_connected(FlEventChannel *channel, FlValue *args, gpointer user_data){  
  is_event_channel_connected = true;
  
  return NULL;
}

static FlMethodErrorResponse *on_clipdata_event_chan_disconnected(FlEventChannel *channel, FlValue *args, gpointer user_data){  
  is_event_channel_connected = false;
  
  return NULL;
}

static void on_event_channel_destroyed(gpointer data){
  is_event_channel_connected = false;
  m_clipdata_event_channel = NULL;
}
/**
 * End of event channel handler
 */

void noko_clipboard_plugin_register_with_registrar(FlPluginRegistrar *registrar)
{
  NokoClipboardPlugin *plugin = NOKO_CLIPBOARD_PLUGIN(
      g_object_new(noko_clipboard_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "noko_clipboard",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);
  
  g_autoptr(FlStandardMethodCodec) ev_codec = fl_standard_method_codec_new();
  g_autoptr(FlEventChannel) clipdata_event_channel = fl_event_channel_new(
    fl_plugin_registrar_get_messenger(registrar)
    , "noko_clipboard/clipdata_event_channel"
  , FL_METHOD_CODEC(ev_codec));

  m_clipdata_event_channel = FL_EVENT_CHANNEL(clipdata_event_channel);
  fl_event_channel_set_stream_handlers(
    clipdata_event_channel
    , on_clipdata_event_chan_connected
    , on_clipdata_event_chan_disconnected
    , NULL 
    , on_event_channel_destroyed);
  
  g_object_unref(plugin);
}
