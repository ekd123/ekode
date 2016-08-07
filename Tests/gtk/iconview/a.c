/**
 * a complete sample for GtkIconView
 * an advanced version of http://zetcode.com/tutorials/gtktutorial/gtkwidgetsII/
 * Compile: clang a.c `pkg-config --cflags --libs gtk+-3.0`
 */
#include <gtk/gtk.h>

enum 
{
    COL_ICON,
    COL_STR,
    NUM_COLS
};

GtkTreeModel * create_empty_model (void);
void           append_icon        (GtkIconView *iconview,
                                   const gchar *str,
                                   GdkPixbuf   *image);
void           ftw_add_icons      (GtkIconView *iconview);

gint 
main (gint argc, 
      gchar *argv[])
{
    GtkWidget *win, *iconview;

    gtk_init (&argc, &argv);

    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    iconview = gtk_icon_view_new_with_model (create_empty_model ());

    gtk_container_add (GTK_CONTAINER (win), iconview);
    ftw_add_icons (GTK_ICON_VIEW (iconview));

    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);

    /* binding is neccessary */
    gtk_icon_view_set_text_column (GTK_ICON_VIEW (iconview), COL_STR);
    gtk_icon_view_set_pixbuf_column (GTK_ICON_VIEW (iconview), COL_ICON);
    gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(iconview), 
                                     GTK_SELECTION_SINGLE);

    g_signal_connect (win, "delete-event", G_CALLBACK (gtk_main_quit), NULL);

    gtk_widget_show_all (win);
    gtk_main ();
    return 0;
}

/* add all .png files into @iconview */
void
ftw_add_icons (GtkIconView *iconview)
{
    GDir *dir;
    const gchar *diritem;

    dir = g_dir_open (".", 0, NULL);
    while ((diritem = g_dir_read_name (dir)))
    {
        if (g_str_has_suffix (diritem, ".png"))
        {
            GdkPixbuf *pixbuf;
            GError *error = NULL;
            pixbuf = gdk_pixbuf_new_from_file (diritem, &error);
            if (pixbuf)
            {
                append_icon (iconview, diritem, pixbuf);
                continue;
            }
            g_warning ("could not load %s because %s\n", diritem, error->message);
            g_error_free (error);
        }
    }
    g_dir_close (dir);
}

/* create an empty tree model */
GtkTreeModel *
create_empty_model (void)
{
    GtkListStore *store;
    /* be sure it's the same order of the enum!!!!!!!!!!!!!!!!!!!! 
       or SIGBUS SIGBUS SIGBUS SIGBUS ARE waiting for YOU */
    store = gtk_list_store_new (NUM_COLS, GDK_TYPE_PIXBUF, G_TYPE_STRING);
    return GTK_TREE_MODEL (store);
}

/* append an icon */
void 
append_icon (GtkIconView *iconview,
             const gchar *str,
             GdkPixbuf   *image)
{
    GtkListStore *store;
    GtkTreeIter iter;
    store = GTK_LIST_STORE (gtk_icon_view_get_model (iconview));
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter, COL_ICON, image, COL_STR, str, -1);
}

