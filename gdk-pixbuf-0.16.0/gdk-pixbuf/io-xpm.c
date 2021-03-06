/* GdkPixbuf library - XPM image loader
 *
 * Copyright (C) 1999 Mark Crichton
 * Copyright (C) 1999 The Free Software Foundation
 *
 * Authors: Mark Crichton <crichton@gimp.org>
 *          Federico Mena-Quintero <federico@gimp.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include "gdk-pixbuf-private.h"
#include "gdk-pixbuf-io.h"
#include "gdk-pixbuf-xlib-private.h"



/* I have must have done something to deserve this.
 * XPM is such a crappy format to handle.
 * This code is an ugly hybred from gdkpixmap.c
 * modified to respect transparent colors.
 * It's still a mess, though.
 */

enum buf_op {
	op_header,
	op_cmap,
	op_body
};

typedef struct {
	gchar *color_string;
	XColor color;
	gint transparent;
} _XPMColor;

struct file_handle {
	FILE *infile;
	gchar *buffer;
	guint buffer_size;
};

struct mem_handle {
	const gchar **data;
	int offset;
};


static gint
xpm_seek_string (FILE *infile, const gchar *str, gint skip_comments)
{
	char instr[1024];

	while (!feof (infile)) {
		if (fscanf (infile, "%1023s", instr) < 0)
			return FALSE;

		if (skip_comments == TRUE && strcmp (instr, "/*") == 0) {
			fscanf (infile, "%1023s", instr);
			while (!feof (infile) && strcmp (instr, "*/") != 0)
				fscanf (infile, "%1023s", instr);
			fscanf (infile, "%1023s", instr);
		}

		if (strcmp (instr, str) == 0)
			return TRUE;
	}

	return FALSE;
}

static gint
xpm_seek_char (FILE *infile, gchar c)
{
	gint b, oldb;

	while ((b = getc (infile)) != EOF) {
		if (c != b && b == '/') {
			b = getc (infile);
			if (b == EOF)
				return FALSE;

			else if (b == '*') {	/* we have a comment */
				b = -1;
				do {
					oldb = b;
					b = getc (infile);
					if (b == EOF)
						return FALSE;
				} while (!(oldb == '*' && b == '/'));
			}
		} else if (c == b)
			return TRUE;
	}

	return FALSE;
}

static gint
xpm_read_string (FILE *infile, gchar **buffer, guint *buffer_size)
{
	gint c;
	guint cnt = 0, bufsiz, ret = FALSE;
	gchar *buf;

	buf = *buffer;
	bufsiz = *buffer_size;
	if (buf == NULL) {
		bufsiz = 10 * sizeof (gchar);
		buf = g_new (gchar, bufsiz);
	}

	do {
		c = getc (infile);
	} while (c != EOF && c != '"');

	if (c != '"')
		goto out;

	while ((c = getc (infile)) != EOF) {
		if (cnt == bufsiz) {
			guint new_size = bufsiz * 2;

			if (new_size > bufsiz)
				bufsiz = new_size;
			else
				goto out;

			buf = g_realloc (buf, bufsiz);
			buf[bufsiz - 1] = '\0';
		}

		if (c != '"')
			buf[cnt++] = c;
		else {
			buf[cnt] = 0;
			ret = TRUE;
			break;
		}
	}

 out:
	buf[bufsiz - 1] = '\0';	/* ensure null termination for errors */
	*buffer = buf;
	*buffer_size = bufsiz;
	return ret;
}

static const gchar *
xpm_skip_whitespaces (const gchar *buffer)
{
	gint32 index = 0;

	while (buffer[index] != 0 && (buffer[index] == 0x20 || buffer[index] == 0x09))
		index++;

	return &buffer[index];
}

static const gchar *
xpm_skip_string (const gchar *buffer)
{
	gint32 index = 0;

	while (buffer[index] != 0 && buffer[index] != 0x20 && buffer[index] != 0x09)
		index++;

	return &buffer[index];
}

/* Xlib crashed once at a color name lengths around 125 */
#define MAX_COLOR_LEN 120

static gchar *
xpm_extract_color (const gchar *buffer)
{
	gint counter, numnames;
	const gchar *ptr = NULL;
        gchar ch, temp[128];
	gchar color[MAX_COLOR_LEN], *retcol;
	gint space;

	counter = 0;
	while (ptr == NULL) {
		if ((buffer[counter] == 'c') || (buffer[counter] == 'g')) {
			ch = buffer[counter + 1];
			if (ch == 0x20 || ch == 0x09)
				ptr = &buffer[counter + 1];
		} else if (buffer[counter] == 0)
			return NULL;

		counter++;
	}
	ptr = xpm_skip_whitespaces (ptr);

	if (ptr[0] == 0)
		return NULL;
	else if (ptr[0] == '#') {
		counter = 1;
		while (ptr[counter] != 0 &&
		       ((ptr[counter] >= '0' && ptr[counter] <= '9') ||
			(ptr[counter] >= 'a' && ptr[counter] <= 'f') ||
			(ptr[counter] >= 'A' && ptr[counter] <= 'F')))
			counter++;
		retcol = g_new (gchar, counter + 1);
		strncpy (retcol, ptr, counter);

		retcol[counter] = 0;

		return retcol;
	}
	color[0] = 0;
	numnames = 0;

	space = MAX_COLOR_LEN - 1;
	while (space > 0) {
		sscanf (ptr, "%127s", temp);

		if (((gint) ptr[0] == 0) ||
		    (strcmp ("s", temp) == 0) || (strcmp ("m", temp) == 0) ||
		    (strcmp ("g", temp) == 0) || (strcmp ("g4", temp) == 0))
			break;
		else {
			if (numnames > 0) {
				space -= 1;
				strcat (color, " ");
			}

			strncat (color, temp, space);
			space -= MIN (space, strlen (temp));
			ptr = xpm_skip_string (ptr);
			ptr = xpm_skip_whitespaces (ptr);
			numnames++;
		}
	}

	retcol = g_strdup (color);
	return retcol;
}

/* (almost) direct copy from gdkpixmap.c... loads an XPM from a file */

static const gchar *
file_buffer (enum buf_op op, gpointer handle)
{
	struct file_handle *h = handle;

	switch (op) {
	case op_header:
		if (xpm_seek_string (h->infile, "XPM", FALSE) != TRUE)
			break;

		if (xpm_seek_char (h->infile, '{') != TRUE)
			break;
		/* Fall through to the next xpm_seek_char. */

	case op_cmap:
		xpm_seek_char (h->infile, '"');
		fseek (h->infile, -1, SEEK_CUR);
		/* Fall through to the xpm_read_string. */

	case op_body:
		xpm_read_string (h->infile, &h->buffer, &h->buffer_size);
		return h->buffer;

	default:
		g_assert_not_reached ();
	}

	return NULL;
}

/* This reads from memory */
static const gchar *
mem_buffer (enum buf_op op, gpointer handle)
{
	struct mem_handle *h = handle;
	switch (op) {
	case op_header:
	case op_cmap:
	case op_body:
                if (h->data[h->offset]) {
                        const gchar* retval;

                        retval = h->data[h->offset];
                        h->offset += 1;
                        return retval;
                }
                break;

	default:
		g_assert_not_reached ();
                break;
	}

	return NULL;
}

/* Destroy notification function for the pixbuf */
static void
free_buffer (guchar *pixels, gpointer data)
{
	free (pixels);
}

static gboolean
xpm_color_parse (const char *spec, XColor *color)
{
	return gdk_pixbuf_parse_color (spec, &color->red, &color->green, &color->blue);
}

/* This function does all the work. */
static GdkPixbuf *
pixbuf_create_from_xpm (const gchar * (*get_buf) (enum buf_op op, gpointer handle), gpointer handle)
{
	gint w, h, n_col, cpp;
	gint cnt, xcnt, ycnt, wbytes, n, ns;
	gint is_trans = FALSE;
	const gchar *buffer;
        gchar *name_buf;
	gchar pixel_str[32];
	GHashTable *color_hash;
	_XPMColor *colors, *color, *fallbackcolor;
	guchar *pixels, *pixtmp;

	fallbackcolor = NULL;

	buffer = (*get_buf) (op_header, handle);
	if (!buffer) {
		g_warning ("No XPM header found");
		return NULL;
	}
	sscanf (buffer, "%d %d %d %d", &w, &h, &n_col, &cpp);
	if (cpp >= 32) {
		g_warning ("XPM has more than 31 chars per pixel.");
		return NULL;
	}

	/* The hash is used for fast lookups of color from chars */
	color_hash = g_hash_table_new (g_str_hash, g_str_equal);

	name_buf = g_new (gchar, n_col * (cpp + 1));
	colors = g_new (_XPMColor, n_col);

	for (cnt = 0; cnt < n_col; cnt++) {
		gchar *color_name;

		buffer = (*get_buf) (op_cmap, handle);
		if (!buffer) {
			g_warning ("Can't load XPM colormap");
			g_hash_table_destroy (color_hash);
			g_free (name_buf);
			g_free (colors);
			return NULL;
		}

		color = &colors[cnt];
		color->color_string = &name_buf[cnt * (cpp + 1)];
		strncpy (color->color_string, buffer, cpp);
		color->color_string[cpp] = 0;
		buffer += strlen (color->color_string);
		color->transparent = FALSE;

		color_name = xpm_extract_color (buffer);

		if ((color_name == NULL) || (g_strcasecmp (color_name, "None") == 0)
		    || (xpm_color_parse (color_name, &color->color) == FALSE)) {
			color->transparent = TRUE;
			is_trans = TRUE;
		}

		g_free (color_name);
		g_hash_table_insert (color_hash, color->color_string, color);

		if (cnt == 0)
			fallbackcolor = color;
	}

	if (is_trans)
		pixels = malloc (w * h * 4);
	else
		pixels = malloc (w * h * 3);

	if (!pixels) {
		g_hash_table_destroy (color_hash);
		g_free (colors);
		g_free (name_buf);
		return NULL;
	}

	wbytes = w * cpp;
	pixtmp = pixels;

	for (ycnt = 0; ycnt < h; ycnt++) {
		buffer = (*get_buf) (op_body, handle);
		if ((!buffer) || (strlen (buffer) < wbytes))
			continue;

		for (n = 0, cnt = 0, xcnt = 0; n < wbytes; n += cpp, xcnt++) {
			strncpy (pixel_str, &buffer[n], cpp);
			pixel_str[cpp] = 0;
			ns = 0;

			color = g_hash_table_lookup (color_hash, pixel_str);

			/* Bad XPM...punt */
			if (!color)
				color = fallbackcolor;

			*pixtmp++ = color->color.red >> 8;
			*pixtmp++ = color->color.green >> 8;
			*pixtmp++ = color->color.blue >> 8;

			if (is_trans && color->transparent)
				*pixtmp++ = 0;
			else if (is_trans)
				*pixtmp++ = 0xFF;
		}
	}

	g_hash_table_destroy (color_hash);
	g_free (colors);
	g_free (name_buf);

	return gdk_pixbuf_new_from_data (pixels, GDK_COLORSPACE_RGB, is_trans, 8,
					 w, h, is_trans ? (w * 4) : (w * 3),
					 free_buffer, NULL);
}

/* Shared library entry point for file loading */
GdkPixbuf *
gdk_pixbuf__xpm_image_load (FILE *f)
{
	GdkPixbuf *pixbuf;
	struct file_handle h;

	memset (&h, 0, sizeof (h));
	h.infile = f;
	pixbuf = pixbuf_create_from_xpm (file_buffer, &h);
	g_free (h.buffer);

	return pixbuf;
}

/* Shared library entry point for memory loading */
GdkPixbuf *
gdk_pixbuf__xpm_image_load_xpm_data (const gchar **data)
{
        GdkPixbuf *pixbuf;
        struct mem_handle h;

        h.data = data;
        h.offset = 0;

	pixbuf = pixbuf_create_from_xpm (mem_buffer, &h);

	return pixbuf;
}

/* Progressive loader */
typedef struct _XPMContext XPMContext;
struct _XPMContext
{
	ModulePreparedNotifyFunc prepare_func;
	ModuleUpdatedNotifyFunc update_func;
	gpointer user_data;

	gchar *tempname;
	FILE *file;
	gboolean all_okay;
};

/*
 * FIXME xpm loading progressively is not properly implemented.
 * Instead we will buffer to a file then load that file when done.
 * This is very broken but it should be relayively simple to fix
 * in the future.
 */
gpointer
gdk_pixbuf__xpm_image_begin_load (ModulePreparedNotifyFunc prepare_func,
				  ModuleUpdatedNotifyFunc update_func,
				  ModuleFrameDoneNotifyFunc frame_done_func,
				  ModuleAnimationDoneNotifyFunc anim_done_func,
				  gpointer user_data)
{
	XPMContext *context;
	gint fd;

	context = g_new (XPMContext, 1);
	context->prepare_func = prepare_func;
	context->update_func = update_func;
	context->user_data = user_data;
	context->all_okay = TRUE;
	context->tempname = g_strdup ("/tmp/gdkpixbuf-xpm-tmp.XXXXXX");
	fd = mkstemp (context->tempname);
	if (fd < 0) {
		g_free (context->tempname);
		g_free (context);
		return NULL;
	}

	context->file = fdopen (fd, "w+");
	if (context->file == NULL) {
		g_free (context->tempname);
		g_free (context);
		return NULL;
	}

	return context;
}

void
gdk_pixbuf__xpm_image_stop_load (gpointer data)
{
	XPMContext *context = (XPMContext*) data;
	GdkPixbuf *pixbuf;

	g_return_if_fail (data != NULL);

	fflush (context->file);
	rewind (context->file);
	if (context->all_okay) {
		pixbuf = gdk_pixbuf__xpm_image_load (context->file);

		(* context->prepare_func) (pixbuf, context->user_data);
		(* context->update_func) (pixbuf, 0, 0, pixbuf->width, pixbuf->height, context->user_data);
		gdk_pixbuf_unref (pixbuf);
	}

	fclose (context->file);
	unlink (context->tempname);
	g_free (context->tempname);
	g_free ((XPMContext *) context);
}

gboolean
gdk_pixbuf__xpm_image_load_increment (gpointer data, guchar *buf, guint size)
{
	XPMContext *context = (XPMContext *) data;

	g_return_val_if_fail (data != NULL, FALSE);

	if (fwrite (buf, sizeof (guchar), size, context->file) != size) {
		context->all_okay = FALSE;
		return FALSE;
	}

	return TRUE;
}
