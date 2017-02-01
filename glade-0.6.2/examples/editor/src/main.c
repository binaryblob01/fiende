/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

int
main (int argc, char *argv[])
{
  GtkWidget *main_window, *statusbar;

  /* This sets the location of the translations of all the text in the
     interface. Glade outputs the coded to set PACKAGE_LOCALE_DIR in
     configure.in, and PACKAGE_LOCALE_DIR is also added to acconfig.h */
  bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
  textdomain (PACKAGE);

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  /* This is automatically output by Glade to set the directories containing
     the pixmaps. The first directory is the place where pixmaps will be
     installed. The second directory is where the uninstalled pixmaps are,
     so the pixmaps can still be found without installing the application.
     Note that for Gnome applications, pixmaps are handled differently, and
     the pixmaps must be installed. PACKAGE_DATA_DIR and PACKAGE_SOURCE_DIR
     are set by configure.in and must appear in acconfig.h */
  add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps");
  add_pixmap_directory (PACKAGE_SOURCE_DIR "/pixmaps");

  main_window = create_main_window ();
  set_window_title (main_window);
  gtk_widget_show (main_window);

  /* We use the support function lookup_widget(), which is provided by Glade
     so you can get a pointer to any widget in a component, simply by passing
     any widget in the component, and the name of the widget you want.
     Here we pass the toplevel widget in the component, main_window, and
     we want to get the status bar widget which is called "statusbar1". */
  statusbar = lookup_widget (main_window, "statusbar1");
  gtk_statusbar_push (GTK_STATUSBAR (statusbar), 1,
		      "Welcome to the Glade Text Editor.");

  gtk_main ();
  return 0;
}

