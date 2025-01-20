#ifndef CLIPBOARD_WATCHER_H
#define CLIPBOARD_WATCHER_H

#include <stdbool.h>
#include <glib-2.0/glib.h>

typedef void (*clipdata_changed_cb)(const gchar *clip_data_text);

bool clip_watcher_initialize(void);

bool clip_watcher_dispose(void);

void clip_watcher_add_clipdata_changed_cb(clipdata_changed_cb cb);

#endif