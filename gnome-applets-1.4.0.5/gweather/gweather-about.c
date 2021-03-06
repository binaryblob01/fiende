/* $Id: gweather-about.c,v 1.4 2000/02/23 08:14:26 peterh Exp $ */

/*
 *  Papadimitriou Spiros <spapadim+@cs.cmu.edu>
 *
 *  This code released under the GNU GPL.
 *  Read the file COPYING for more information.
 *
 *  About box
 *
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <gnome.h>

#include "gweather-about.h"


void gweather_about_run (void)
{
    const gchar *authors[] = {
        "Spiros Papadimitriou <spapadim+@cs.cmu.edu>",
        NULL
    };
    static GtkWidget *about_dialog = NULL;
    
    if (about_dialog != NULL)
    {
    	gdk_window_show(about_dialog->window);
    	gdk_window_raise(about_dialog->window);
	return;
    }
    about_dialog = gnome_about_new ("GNOME Weather", "0.05",
                                    _("Copyright (c)1999 by S. Papadimitriou"),
                                    authors,
                                    _("GNOME weather monitor applet.\nWeb: http://gweather.dhs.org/"),
                                    NULL);
    gtk_signal_connect( GTK_OBJECT(about_dialog), "destroy",
		        GTK_SIGNAL_FUNC(gtk_widget_destroyed), &about_dialog );
    gtk_widget_show(about_dialog);
    
    
    return;
}
