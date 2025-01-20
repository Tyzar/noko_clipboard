#include "clipboard_watcher.h"
#include <pthread.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

const int MICRO_TO_MILIS_SEC = 1000;

static bool is_running = false;
static gchar *clip_data_text = NULL;
static clipdata_changed_cb _clipdata_cb = NULL;

static gboolean notify_clipdata_changed(gpointer data)
{
    if (_clipdata_cb)
    {
        _clipdata_cb(clip_data_text);
    }

    return G_SOURCE_REMOVE;
}

static void *observe_clipboard(void *arg)
{
    GtkClipboard *gtkcb = GTK_CLIPBOARD(arg);
    is_running = true;
    while (is_running)
    {
        usleep(1000 * MICRO_TO_MILIS_SEC); // sleep thread
        printf("Try get clip data...\n");

        gchar *new_clip_text = gtk_clipboard_wait_for_text(gtkcb);
        if (!new_clip_text)
        {
            continue;
        }

        if (clip_data_text && g_strcmp0(clip_data_text, new_clip_text) == 0)
        {
            continue;
        }

        printf("New clip text: %s\n", new_clip_text);
        clip_data_text = strdup(new_clip_text);
        g_free(new_clip_text);

        // notify to main gtk context thread
        g_idle_add(G_SOURCE_FUNC(notify_clipdata_changed), NULL);
    }

    printf("Stopping clip watcher...\n");
    return NULL;
}

/**
 * Called in gtk main thread
 */
void clip_watcher_add_clipdata_changed_cb(clipdata_changed_cb cb)
{
    _clipdata_cb = cb;
}

bool clip_watcher_initialize(void)
{
    if (is_running)
    {
        return true;
    }

    GtkClipboard *gtkcb = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    if (!gtkcb)
    {
        return false;
    }

    pthread_t exec_thread;
    pthread_create(&exec_thread, NULL, observe_clipboard, gtkcb);
    return true;
}

bool clip_watcher_dispose(void)
{
    _clipdata_cb = NULL;
    is_running = false;
    if (clip_data_text)
    {
        g_free(clip_data_text);
        clip_data_text = NULL;
    }
    printf("Clip watcher is disposed...\n");
    return true;
}