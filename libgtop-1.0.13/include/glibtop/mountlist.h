/* $Id: mountlist.h,v 1.11 1999/02/23 11:42:57 martin Exp $ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LibGTop; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __GLIBTOP_MOUNTLIST_H__
#define __GLIBTOP_MOUNTLIST_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_MOUNTLIST_NUMBER	0
#define GLIBTOP_MOUNTLIST_TOTAL		1
#define GLIBTOP_MOUNTLIST_SIZE		2

#define GLIBTOP_MAX_MOUNTLIST		3

typedef struct _glibtop_mountentry	glibtop_mountentry;

typedef struct _glibtop_mountlist	glibtop_mountlist;

struct _glibtop_mountentry
{
	u_int64_t dev;
	char devname [GLIBTOP_MOUNTENTRY_LEN+1];
	char mountdir [GLIBTOP_MOUNTENTRY_LEN+1];
	char type [GLIBTOP_MOUNTENTRY_LEN+1];
};

struct _glibtop_mountlist
{
	u_int64_t	flags,
		number,			/* GLIBTOP_MOUNTLIST_NUMBER	*/
		total,			/* GLIBTOP_MOUNTLIST_TOTAL	*/
		size;			/* GLIBTOP_MOUNTLIST_SIZE	*/
};

#define glibtop_get_mountlist(mountlist,all_fs)	glibtop_get_mountlist_l(glibtop_global_server, mountlist, all_fs)

#define glibtop_get_mountlist_r		glibtop_get_mountlist_s

glibtop_mountentry *
glibtop_get_mountlist_l (glibtop *server, glibtop_mountlist *buf, int all_fs);

glibtop_mountentry *
glibtop_get_mountlist_s (glibtop *server, glibtop_mountlist *buf, int all_fs);

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_mountlist [];
extern const unsigned glibtop_types_mountlist [];
extern const char *glibtop_labels_mountlist [];
extern const char *glibtop_descriptions_mountlist [];

#endif

END_LIBGTOP_DECLS

#endif
