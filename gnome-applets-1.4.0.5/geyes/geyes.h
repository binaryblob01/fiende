/*
 * Copyright (C) 1999 Dave Camp <dave@davec.dhs.org>
 *  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *  
 */

#ifndef __GEYES_H__
#define __GEYES_H__
#include <gnome.h>
#include <applet-widget.h>

#define MAX_EYES 5
typedef struct
{
    GtkWidget *pbox;
    GtkWidget *clist;
    
    gint selected_row;
} EyesPropertyBox;

typedef struct 
{
    /* Applet */
    GtkWidget *applet;
    GtkWidget *fixed;
    GtkWidget *hbox;
    GtkWidget *eyes[MAX_EYES];
    GdkPixmap *pixmap[MAX_EYES];

    /* Theme */
    GdkImlibImage *eye_image;
    GdkImlibImage *pupil_image;
    gchar *theme_dir;
    gchar *theme_name;
    gchar *eye_filename;
    gchar *pupil_filename;
    gint num_eyes;
    gint eye_height;
    gint eye_width;
    gint pupil_height;
    gint pupil_width;
    gint wall_thickness;

    /* Properties */
    EyesPropertyBox prop_box;
} EyesApplet;

void create_eyes (void);
void destroy_eyes (void);

void load_theme (gchar *theme_name);
void properties_cb (AppletWidget *applet, gpointer data);
#endif
