#ifndef lint
static char rcsId[]="$Header: /cvs/gnome/gnome-libs/gtk-xmhtml/Attic/readFLG.c,v 1.4.14.1 2002/01/08 22:33:11 kmaraas Exp $";
#endif
/*****
* readFLG.c : XmHTML Fast Loadable Graphics reader.
*
* This file Version	$Revision: 1.4.14.1 $
*
* Creation date:		Sun Sep 28 14:11:03 GMT+0100 1997
* Last modification: 	$Date: 2002/01/08 22:33:11 $
* By:					$Author: kmaraas $
* Current State:		$State: Exp $
*
* Author:				newt
*
* Copyright (C) 1994-1997 by Ripley Software Development 
* All Rights Reserved
*
* This file is part of the XmHTML Widget Library.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU [Library] General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU [Library] General Public
* License along with this library; if not, write to the Free
* Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*****/
/*****
* ChangeLog 
* $Log: readFLG.c,v $
* Revision 1.4.14.1  2002/01/08 22:33:11  kmaraas
* 2002-01-06  Kjartan Maraas  <kmaraas@gnome.org>
*
* 	* *: Fix compiler warnings.
* 	* images.c: Fix missing X color context ref that was causing lots
* 	of crashes. Fixes #60237, #61638, #63439, #65040, #66913 and more.
* 	* test.c: do not use %s for a boolean use %d instead.
*
* Revision 1.4  1998/02/12 03:09:44  unammx
* Merge to Koen's XmHTML 1.1.2 + following fixes:
*
* Wed Feb 11 20:27:19 1998  Miguel de Icaza  <miguel@nuclecu.unam.mx>
*
* 	* gtk-forms.c (freeForm): gtk_destroy_widget is no longer needed
* 	with the refcounting changes.
*
* 	* gtk-xmhtml.c (gtk_xmhtml_remove): Only god knows why I was
* 	adding the just removed widget.
*
* Revision 1.3  1998/01/07 01:45:40  unammx
* Gtk/XmHTML is ready to be used by the Gnome hackers now!
* Weeeeeee!
*
* This afternoon:
*
* 	- Changes to integrate gtk-xmhtml into an autoconf setup.
*
* 	- Changes to make gtk-xmhtml a library to be used by Gnome
* 	  (simply include <gtk-xmhtml/gtk-xmhtml.h and link
* 	   with -lgtkxmhtml and you are set).
*
* Revision 1.2  1997/12/18 22:13:20  unammx
* Internal image format now compiles.
*
* Revision 1.1  1997/10/23 00:23:22  newt
* Initial Revision
*
*****/ 
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(HAVE_LIBPNG) || defined(HAVE_LIBZ)
#include <zlib.h>
#endif

#include "XmHTMLP.h"
#include "XmHTMLfuncs.h"

/*** External Function Prototype Declarations ***/

/*** Public Variable Declarations ***/

/*** Private Datatype Declarations ****/

/*** Private Function Prototype Declarations ****/

/*** Private Variable Declarations ***/

#define _readByte(VAL) do{ \
	VAL = (*ib->curr_pos++ & 0xff); \
	ib->next++; \
}while(0)

#define _readDimension(VAL) do{ \
	VAL = (*ib->curr_pos++ & 0xff); \
	VAL |= ((*ib->curr_pos++ & 0xff) << 8); \
	ib->next += 2; \
}while(0)

#define _readCardinal(VAL) do{ \
	VAL = (*ib->curr_pos++ & 0xff); \
	VAL |= ((*ib->curr_pos++ & 0xff) << 8); \
	VAL |= ((*ib->curr_pos++ & 0xff) << 16); \
	VAL |= ((*ib->curr_pos++ & 0xff) << 24); \
	ib->next += 4; \
}while(0)

#define _readInt(VAL) do{ \
	VAL = (*ib->curr_pos++ & 0xff); \
	VAL |= ((*ib->curr_pos++ & 0xff) << 8); \
	VAL |= ((*ib->curr_pos++ & 0xff) << 16); \
	VAL |= ((*ib->curr_pos++ & 0xff) << 24); \
	ib->next += 4; \
}while(0)

static XmImageInfo*
readFLG(ImageBuffer *ib)
{
	XmImageInfo *all_frames, *frame, *current;
	int frameCount, i;
	Byte c, *data;
	Dimension *color;
	Cardinal nframes, loop_count;

	/* bogus frame leader */
	all_frames = (XmImageInfo*)malloc(sizeof(XmImageInfo));
	all_frames->frame = NULL;
	current = all_frames;

	/* skip magic */
	ib->next += 6;
	ib->curr_pos += 6;

	/* FLG revision */
	_readByte(c);

	/* compress byte */
	_readByte(c);

	/* original image type */
	_readByte(c);

	/* no of frames in this file */
	_readCardinal(nframes);

	/* no of times to repeat animation */
	_readCardinal(loop_count);

	/* read all frames */
	frameCount = 0;

	do{
		frame = (XmImageInfo*)calloc(1, sizeof(XmImageInfo));

		/* default options */
		frame->options = XmIMAGE_DEFERRED_FREE | XmIMAGE_RGB_SINGLE |
							XmIMAGE_ALLOW_SCALE;

		frame->type = IMAGE_FLG;

		/* frames remaining */
		frame->nframes = nframes - frameCount;
		frame->loop_count = loop_count;

		_readDimension(frame->width);
		_readDimension(frame->height);
		frame->swidth  = frame->width;
		frame->sheight = frame->height;

		/* logical screen offsets (these can be negative) */
		_readInt(frame->x);
		_readInt(frame->y);

		/* frame timeout */
		_readCardinal(frame->timeout);

		/* frame disposal method */
		_readByte(frame->dispose);

		/* frame depth (original bits per pixel) */
		_readByte(frame->depth);

		/* transparent pixel index */
		_readInt(frame->bg);

		/* transparency type */
		_readByte(frame->transparency);

		/* colorspace */
		_readByte(frame->colorspace);

		/* size of colormap */
		_readCardinal(frame->ncolors);

		if(frame->ncolors)
		{
			/* add one, it's zero based */
			frame->ncolors++;
			frame->scolors = frame->ncolors;

			/* alloc colormap */
			frame->reds = (Dimension*)calloc(3*frame->ncolors,
							sizeof(Dimension));

			/* read it */
			color = frame->reds;

			/* red color components */
			for(i = 0; i < frame->ncolors; i++, color++)
				_readDimension(*color);

			/* green color components */
			frame->greens = color;
			for(i = 0; i < frame->ncolors; i++, color++)
				_readDimension(*color);

			/* blue color components */
			frame->blues = color;
			for(i = 0; i < frame->ncolors; i++, color++)
				_readDimension(*color);
		}

		/* frame data */
		frame->data = (Byte*)malloc(frame->width * frame->height);
		data = frame->data;
		for(i = 0; i < frame->width * frame->height; i++)
			_readByte(*data++);

		/* clipmask data (if we have got one) */
		if(frame->bg != -1 && frame->transparency == XmIMAGE_TRANSPARENCY_BG)
		{
			int wd;
			Byte *clip;

			wd = frame->width;
			/* pad so it will be a multiple of 8 */
			while((wd % 8))
				wd++;

			/* this many bytes on a row */
			wd /= 8;
			/* this many bytes in the clipmask */
			wd *= frame->height;

			frame->clip = (Byte*)malloc(wd);
			clip = frame->clip;
			for(i = 0; i < wd; i++)
				_readByte(*clip++);

			/* we have a clipmask */
			frame->options |= XmIMAGE_CLIPMASK;
		}

		/* alpha channel (if any) */
		if(frame->transparency == XmIMAGE_TRANSPARENCY_ALPHA)
		{
			Cardinal gamma;
			Byte *alpha;

			/* frame gamma */
			_readCardinal(gamma);
			frame->fg_gamma = (float)gamma/100000.0;

			/* write out alpha channel */
			frame->alpha = (Byte*)malloc(frame->width * frame->height);
			alpha = frame->alpha;
			for(i = 0; i < frame->width * frame->height; i++)
				_readByte(*alpha++);
		}
		/* terminator */
		_readByte(c);

		if(c != 0)
			fprintf(stderr, "readFLG: missing separator bit on frame %i!\n",
				frameCount);

		current->frame = frame;
		current = current->frame;

		frameCount++;	/* done with this frame */
	}while(frameCount < nframes);

	current = all_frames->frame;
	free(all_frames);
	return(current);
}


/*****
* Name:			_XmHTMLReadFLG
* Return Type:	XmImage*
* Description:	reads a Fast Loadable Graphic directly into an XmImage.
* In:
*	ib:			raw image data;
* Returns:
*	an allocated XmImage or NULL on failure.
*****/
XmImageInfo*
_XmHTMLReadFLG(XmHTMLWidget html, ImageBuffer *ib)
{
	XmImageInfo *image = NULL;
	Byte *buffer = NULL, c = 0;
	Boolean err = False;
	ImageBuffer *dp;

#if defined(HAVE_LIBPNG) || defined(HAVE_LIBZ)
	ImageBuffer data;
	int zlib_err;
#endif

	image = NULL;

	/* sanity */
	RewindImageBuffer(ib);

	/* skip magic & version number */
	ib->curr_pos += 7;
	ib->next += 7;

	/* check if data is compressed */
	_readByte(c);

	if(c == 1)
	{
#if !defined(HAVE_LIBPNG) && !defined(HAVE_LIBZ)
		/* compressed FLG requires zlib support to be present */
		_XmHTMLWarning(__WFUNC__(html, "_XmHTMLReadFLG"),
			"%s: can't uncompress:\n    Reason: zlib support not present.",
			ib->file); 
		return(image);
#else
		Cardinal tmp = 0;
		unsigned long dsize, csize;

		/* get size of uncompressed data */
		_readCardinal(tmp);
		dsize = (unsigned long)tmp;

		/* size of compressed data */
		csize = (unsigned long)(ib->size - ib->next);

		/* allocate uncompress buffer */
		buffer = (Byte*)malloc(dsize+1);

		if((zlib_err = uncompress(buffer, &dsize, ib->curr_pos, csize)) != Z_OK)
		{
			_XmHTMLWarning(__WFUNC__(html, "_XmHTMLReadFLG"),
				"%s: uncompress failed\n    Reason: %s.", ib->file,
				zlib_err == Z_MEM_ERROR ? "out of memory" :
				zlib_err == Z_BUF_ERROR ? "not enough output room" :
				"data corrupted");
			err = True;
		}
		else if(dsize != (unsigned long)tmp)
		{
			_XmHTMLWarning(__WFUNC__(html, "_XmHTMLReadFLG"),
				"%s: uncompress failed\n    %li bytes returned while %i "
				"are expected. Data corrupt?", ib->file,
				"%s: to uncompress data.", dsize, tmp);
			err = True;
		}
		/* compose local image buffer */
		data.buffer = buffer;
		data.curr_pos = data.buffer;
		data.size   = (size_t)dsize;
		data.next   = 0;
		data.may_free = False;
		dp = &data;
#endif
	}
	else
		dp = ib;

	if(err)
	{
		/* free up compressed buffer */
		if(c == 1)
			free(buffer);
		return(image);
	}

	/* now go and process the actual image */
	RewindImageBuffer(dp);

	image = readFLG(dp);

	/* store name of image */
	image->url = strdup(ib->file);	/* image location */

	/* free up compressed buffer */
	if(c == 1)
		free(buffer);

	return(image);
}
