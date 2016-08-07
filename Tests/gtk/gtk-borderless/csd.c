/*
   client-side decoration 
   some from OSD Lyrics by Tiger Soldier

   https://github.com/osdlyrics/osdlyrics/blob/master/src/ol_scroll_window.c
*/
#include <gtk/gtk.h>

static GtkWidget *window;
static GtkWidget *eventbox; /* used for title bar */
static GtkWidget *box;
static GtkWidget *label; /* used for title bar */

static gboolean _button_press ();
static gboolean window_csd_begin_move_resize ();

/*
  spacing: the number of pixels to place by default between the bar and the contents panel.
*/
GtkWidget *
gtk_csd_window_new (gint spacing)
{
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_assert (window != NULL);
    gtk_window_set_decorated (GTK_WINDOW (window), FALSE);

    /* and bar and app's contents panel */
    box = gtk_box_new (GTK_ORIENTATION_VERTICAL,
                       spacing);
    label = gtk_label_new ("WINDOW CSD");
    eventbox = gtk_event_box_new ();
    g_assert (box && label && eventbox);

    //gtk_container_add (GTK_CONTAINER (eventbox), label);
    gtk_event_box_set_above_child (eventbox, TRUE);

    gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);

    /* handle mouse events */
    gtk_widget_add_events (GTK_WIDGET (window), 
                           GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK |
                           GDK_POINTER_MOTION_MASK |
                           GDK_POINTER_MOTION_HINT_MASK);
    g_signal_connect (window, "button-press-event", 
                      G_CALLBACK (_button_press), window);

    return window;
}

void
gtk_window_csd_add_default_title_bar ()
{
    gchar *title;
    g_object_get (window, "title", &title, NULL);
    gtk_label_set_text (GTK_LABEL (label), title);
    g_free (title);
}

static gboolean
_button_press (GtkWidget *widget,
               GdkEventButton *event)
{
    if (event->button == 1)
        window_csd_begin_move_resize (widget, event);
    return FALSE;
}

static gboolean 
window_csd_begin_move_resize (GtkWidget *widget,
                              GdkEventButton *event)
{
    gint width, height, x, y;

    gtk_window_begin_move_drag (GTK_WINDOW (window), event->button,
                                (gint)event->x_root,
                                (gint)event->y_root,
                                event->time);

    return TRUE;
}
