/* GdkPixbuf library - JPEG image loader
 *
 * Copyright (C) 1999 Michael Zucchi
 * Copyright (C) 1999 The Free Software Foundation
 * 
 * Progressive loading code Copyright (C) 1999 Red Hat, Inc.
 *
 * Authors: Michael Zucchi <zucchi@zedzone.mmc.com.au>
 *          Federico Mena-Quintero <federico@gimp.org>
 *          Michael Fulbright <drmike@redhat.com>
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


/*
  Progressive file loading notes (11/03/1999) <drmike@redhat.com>...

  These are issues I know of and will be dealing with shortly:

    -  Currently does not handle progressive jpegs - this
       requires a change in the way image_load_increment () calls
       libjpeg. Progressive jpegs are rarer but I will add this
       support asap.

    - error handling is not as good as it should be

 */


#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <jpeglib.h>
#include "gdk-pixbuf-private.h"
#include "gdk-pixbuf-io.h"



/* we are a "source manager" as far as libjpeg is concerned */
#define JPEG_PROG_BUF_SIZE 4096

typedef struct {
	struct jpeg_source_mgr pub;   /* public fields */

	JOCTET buffer[JPEG_PROG_BUF_SIZE];              /* start of buffer */
	long  skip_next;              /* number of bytes to skip next read */

} my_source_mgr;

typedef my_source_mgr * my_src_ptr;

/* error handler data */
struct error_handler_data {
	struct jpeg_error_mgr pub;
	sigjmp_buf setjmp_buffer;
};

/* progressive loader context */
typedef struct {
	ModuleUpdatedNotifyFunc  updated_func;
	ModulePreparedNotifyFunc prepared_func;
	gpointer                 user_data;
	
	GdkPixbuf                *pixbuf;
	guchar                   *dptr;   /* current position in pixbuf */

	gboolean                 did_prescan;  /* are we in image data yet? */
	gboolean                 got_header;  /* have we loaded jpeg header? */
	gboolean                 src_initialized;/* TRUE when jpeg lib initialized */
	struct jpeg_decompress_struct cinfo;
	struct error_handler_data     jerr;
} JpegProgContext;

GdkPixbuf *gdk_pixbuf__jpeg_image_load (FILE *f);
gpointer gdk_pixbuf__jpeg_image_begin_load (ModulePreparedNotifyFunc func, 
					    ModuleUpdatedNotifyFunc func2,
					    ModuleFrameDoneNotifyFunc func3,
					    ModuleAnimationDoneNotifyFunc func4,
					    gpointer user_data);
void gdk_pixbuf__jpeg_image_stop_load (gpointer context);
gboolean gdk_pixbuf__jpeg_image_load_increment(gpointer context, guchar *buf, guint size);


static void
fatal_error_handler (j_common_ptr cinfo)
{
	/* FIXME:
	 * We should somehow signal what error occurred to the caller so the
	 * caller can handle the error message */
	struct error_handler_data *errmgr;

	errmgr = (struct error_handler_data *) cinfo->err;
	cinfo->err->output_message (cinfo);
	siglongjmp (errmgr->setjmp_buffer, 1);

	/* incase the jmp buf isn't initted? */
	exit(1);
}

/* Destroy notification function for the pixbuf */
static void
free_buffer (guchar *pixels, gpointer data)
{
	free (pixels);
}


/* explode gray image data from jpeg library into rgb components in pixbuf */
static void
explode_gray_into_buf (struct jpeg_decompress_struct *cinfo,
		       guchar **lines) 
{
	gint i, j;
	guint w;

	g_return_if_fail (cinfo != NULL);
	g_return_if_fail (cinfo->output_components == 1);

	/* Expand grey->colour.  Expand from the end of the
	 * memory down, so we can use the same buffer.
	 */
	w = cinfo->image_width;
	for (i = cinfo->rec_outbuf_height - 1; i >= 0; i--) {
		guchar *from, *to;
		
		from = lines[i] + w - 1;
		to = lines[i] + (w - 1) * 3;
		for (j = w - 1; j >= 0; j--) {
			to[0] = from[0];
			to[1] = from[0];
			to[2] = from[0];
			to -= 3;
			from--;
		}
	}
}

/* Shared library entry point */
GdkPixbuf *
gdk_pixbuf__jpeg_image_load (FILE *f)
{
	gint w, h, i;
	guchar *pixels = NULL;
	guchar *dptr;
	guchar *lines[4]; /* Used to expand rows, via rec_outbuf_height, 
                           * from the header file: 
                           * " Usually rec_outbuf_height will be 1 or 2, 
                           * at most 4."
			   */
	guchar **lptr;
	struct jpeg_decompress_struct cinfo;
	struct error_handler_data jerr;

	/* setup error handler */
	cinfo.err = jpeg_std_error (&jerr.pub);
	jerr.pub.error_exit = fatal_error_handler;

	if (sigsetjmp (jerr.setjmp_buffer, 1)) {
		/* Whoops there was a jpeg error */
		if (pixels)
			free (pixels);

		jpeg_destroy_decompress (&cinfo);
		return NULL;
	}

	/* load header, setup */
	jpeg_create_decompress (&cinfo);
	jpeg_stdio_src (&cinfo, f);
	jpeg_read_header (&cinfo, TRUE);
	jpeg_start_decompress (&cinfo);
	cinfo.do_fancy_upsampling = FALSE;
	cinfo.do_block_smoothing = FALSE;

	w = cinfo.output_width;
	h = cinfo.output_height;

	pixels = malloc (h * w * 3);
	if (!pixels) {
		jpeg_destroy_decompress (&cinfo);
		return NULL;
	}

	dptr = pixels;

	/* decompress all the lines, a few at a time */

	while (cinfo.output_scanline < cinfo.output_height) {
		lptr = lines;
		for (i = 0; i < cinfo.rec_outbuf_height; i++) {
			*lptr++ = dptr;
			dptr += w * 3;
		}

		jpeg_read_scanlines (&cinfo, lines, cinfo.rec_outbuf_height);

		if (cinfo.output_components == 1)
			explode_gray_into_buf (&cinfo, lines);
	}

	jpeg_finish_decompress (&cinfo);
	jpeg_destroy_decompress (&cinfo);

	return gdk_pixbuf_new_from_data (pixels, GDK_COLORSPACE_RGB, FALSE, 8,
					 w, h, w * 3,
					 free_buffer, NULL);
}


/**** Progressive image loading handling *****/

/* these routines required because we are acting as a source manager for */
/* libjpeg. */
static void
init_source (j_decompress_ptr cinfo)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;

	src->skip_next = 0;
}


static void
term_source (j_decompress_ptr cinfo)
{
	/* XXXX - probably should scream something has happened */
}


/* for progressive loading (called "I/O Suspension" by libjpeg docs) */
/* we do nothing except return "FALSE"                               */
static boolean
fill_input_buffer (j_decompress_ptr cinfo)
{
	return FALSE;
}


static void
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;
	long   num_can_do;

	/* move as far as we can into current buffer */
	/* then set skip_next to catch the rest      */
	if (num_bytes > 0) {
		num_can_do = MIN (src->pub.bytes_in_buffer, num_bytes);
		src->pub.next_input_byte += (size_t) num_can_do;
		src->pub.bytes_in_buffer -= (size_t) num_can_do;

		src->skip_next = num_bytes - num_can_do;
	}
}

 
/* 
 * func - called when we have pixmap created (but no image data)
 * user_data - passed as arg 1 to func
 * return context (opaque to user)
 */

gpointer
gdk_pixbuf__jpeg_image_begin_load (ModulePreparedNotifyFunc prepared_func, 
				   ModuleUpdatedNotifyFunc  updated_func,
				   ModuleFrameDoneNotifyFunc frame_func,
				   ModuleAnimationDoneNotifyFunc anim_done_func,
				   gpointer user_data)
{
	JpegProgContext *context;
	my_source_mgr   *src;

	context = g_new0 (JpegProgContext, 1);
	context->prepared_func = prepared_func;
	context->updated_func  = updated_func;
	context->user_data = user_data;
	context->pixbuf = NULL;
	context->got_header = FALSE;
	context->did_prescan = FALSE;
	context->src_initialized = FALSE;

	/* create libjpeg structures */
	jpeg_create_decompress (&context->cinfo);

	context->cinfo.src = (struct jpeg_source_mgr *) g_new0 (my_source_mgr, 1);
	src = (my_src_ptr) context->cinfo.src;

	context->cinfo.err = jpeg_std_error (&context->jerr.pub);
	context->jerr.pub.error_exit = fatal_error_handler;

	src = (my_src_ptr) context->cinfo.src;
	src->pub.init_source = init_source;
	src->pub.fill_input_buffer = fill_input_buffer;
	src->pub.skip_input_data = skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart;
	src->pub.term_source = term_source;
	src->pub.bytes_in_buffer = 0;
	src->pub.next_input_byte = NULL;

	return (gpointer) context;
}

/*
 * context - returned from image_begin_load
 *
 * free context, unref gdk_pixbuf
 */
void
gdk_pixbuf__jpeg_image_stop_load (gpointer data)
{
	JpegProgContext *context = (JpegProgContext *) data;

	g_return_if_fail (context != NULL);

	if (context->pixbuf)
		gdk_pixbuf_unref (context->pixbuf);

	/* if we have an error? */
	if (sigsetjmp (context->jerr.setjmp_buffer, 1)) {
		jpeg_destroy_decompress (&context->cinfo);
	} else {
		jpeg_finish_decompress(&context->cinfo);
		jpeg_destroy_decompress(&context->cinfo);
	}

	if (context->cinfo.src) {
		my_src_ptr src = (my_src_ptr) context->cinfo.src;
		
		g_free (src);
	}

	g_free (context);
}




/*
 * context - from image_begin_load
 * buf - new image data
 * size - length of new image data
 *
 * append image data onto inrecrementally built output image
 */
gboolean
gdk_pixbuf__jpeg_image_load_increment (gpointer data, guchar *buf, guint size)
{
	JpegProgContext *context = (JpegProgContext *)data;
	struct jpeg_decompress_struct *cinfo;
	my_src_ptr  src;
	guint       num_left, num_copy;
	guint       last_bytes_left;
	guint       spinguard;
	gboolean    first;
	guchar *bufhd;

	g_return_val_if_fail (context != NULL, FALSE);
	g_return_val_if_fail (buf != NULL, FALSE);

	src = (my_src_ptr) context->cinfo.src;

	cinfo = &context->cinfo;

	/* XXXXXXX (drmike) - loop(s) below need to be recoded now I
         *                    have a grasp of what the flow needs to be!
         */

	/* check for fatal error */
	if (sigsetjmp (context->jerr.setjmp_buffer, 1)) {
		return FALSE;
	}

	/* skip over data if requested, handle unsigned int sizes cleanly */
	/* only can happen if we've already called jpeg_get_header once   */
	if (context->src_initialized && src->skip_next) {
		if (src->skip_next > size) {
			src->skip_next -= size;
			return TRUE;
		} else {
			num_left = size - src->skip_next;
			bufhd = buf + src->skip_next;
			src->skip_next = 0;
		}
	} else {
		num_left = size;
		bufhd = buf;
	}

	if (num_left == 0)
		return TRUE;

	last_bytes_left = 0;
	spinguard = 0;
	first = TRUE;
	while (TRUE) {

		/* handle any data from caller we haven't processed yet */
		if (num_left > 0) {
			if(src->pub.bytes_in_buffer && 
			   src->pub.next_input_byte != src->buffer)
				memmove(src->buffer, src->pub.next_input_byte,
					src->pub.bytes_in_buffer);


			num_copy = MIN (JPEG_PROG_BUF_SIZE - src->pub.bytes_in_buffer,
					num_left);

/*			if (num_copy == 0) 
				g_error ("Buffer overflow!");
*/
			memcpy(src->buffer + src->pub.bytes_in_buffer, bufhd,num_copy);
			src->pub.next_input_byte = src->buffer;
			src->pub.bytes_in_buffer += num_copy;
			bufhd += num_copy;
			num_left -= num_copy;
		} else {
		/* did anything change from last pass, if not return */
			if (first) {
				last_bytes_left = src->pub.bytes_in_buffer;
				first = FALSE;
			} else if (src->pub.bytes_in_buffer == last_bytes_left)
				spinguard++;
			else
				last_bytes_left = src->pub.bytes_in_buffer;
		}

		/* should not go through twice and not pull bytes out of buf */
		if (spinguard > 2)
			return TRUE;

		/* try to load jpeg header */
		if (!context->got_header) {
			int rc;

			rc = jpeg_read_header (cinfo, TRUE);
			context->src_initialized = TRUE;

			if (rc == JPEG_SUSPENDED)
				continue;

			context->got_header = TRUE;

#if 0
			if (jpeg_has_multiple_scans (cinfo)) {
				g_print ("io-jpeg.c: Does not currently "
					 "support progressive jpeg files.\n");
				return FALSE;
			}
#endif
			context->pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 
							 FALSE,
							 8, 
							 cinfo->image_width,
							 cinfo->image_height);

			if (context->pixbuf == NULL) {
				/* Failed to allocate memory */
				g_error ("Couldn't allocate gdkpixbuf");
			}

			/* Use pixbuf buffer to store decompressed data */
			context->dptr = context->pixbuf->pixels;

			/* Notify the client that we are ready to go */
			(* context->prepared_func) (context->pixbuf,
						    context->user_data);

		} else if (!context->did_prescan) {
			int rc;

			/* start decompression */
			rc = jpeg_start_decompress (cinfo);
			cinfo->do_fancy_upsampling = FALSE;
			cinfo->do_block_smoothing = FALSE;

			if (rc == JPEG_SUSPENDED)
				continue;

			context->did_prescan = TRUE;
		} else {

			/* we're decompressing so feed jpeg lib scanlines */
			guchar *lines[4];
			guchar **lptr;
			guchar *rowptr;
			gint   nlines, i;
			gint   start_scanline;

			/* keep going until we've done all scanlines */
			while (cinfo->output_scanline < cinfo->output_height) {
				start_scanline = cinfo->output_scanline;
				lptr = lines;
				rowptr = context->dptr;
				for (i=0; i < cinfo->rec_outbuf_height; i++) {
					*lptr++ = rowptr;
					rowptr += context->pixbuf->rowstride;
				}

				nlines = jpeg_read_scanlines (cinfo, lines,
							      cinfo->rec_outbuf_height);
				if (nlines == 0)
					break;

				/* handle gray */
				if (cinfo->output_components == 1)
					explode_gray_into_buf (cinfo, lines);

				context->dptr += nlines * context->pixbuf->rowstride;

				/* send updated signal */
				(* context->updated_func) (context->pixbuf,
							   0, 
							   cinfo->output_scanline-1,
							   cinfo->image_width, 
							   nlines,
							   context->user_data);

#undef DEBUG_JPEG_PROGRESSIVE
#ifdef DEBUG_JPEG_PROGRESSIVE
				
				if (start_scanline != cinfo->output_scanline)
					g_print("jpeg: Input pass=%2d, next input scanline=%3d,"
						" emitted %3d - %3d\n",
						cinfo->input_scan_number, cinfo->input_iMCU_row * 16,
						start_scanline, cinfo->output_scanline - 1);
				
				
				
				g_print ("Scanline %d of %d - ", 
					 cinfo->output_scanline,
					 cinfo->output_height);
/*			g_print ("rec_height %d -", cinfo->rec_outbuf_height); */
				g_print ("Processed %d lines - bytes left = %d\n",
					 nlines, cinfo->src->bytes_in_buffer);
#endif
			}
			/* did entire image */
			if (cinfo->output_scanline >= cinfo->output_height)
				return TRUE;
			else
				continue;
		}
	}

	return TRUE;
}
