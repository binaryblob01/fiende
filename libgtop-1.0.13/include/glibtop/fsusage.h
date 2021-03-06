/* $Id: fsusage.h,v 1.8 1999/02/23 11:42:52 martin Exp $ */

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

#ifndef __GLIBTOP_FSUSAGE_H__
#define __GLIBTOP_FSUSAGE_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_FSUSAGE_BLOCKS		0
#define GLIBTOP_FSUSAGE_BFREE		1
#define GLIBTOP_FSUSAGE_BAVAIL		2
#define GLIBTOP_FSUSAGE_FILES		3
#define GLIBTOP_FSUSAGE_FFREE		4

#define GLIBTOP_MAX_FSUSAGE		5

typedef struct _glibtop_fsusage		glibtop_fsusage;

struct _glibtop_fsusage
{
	u_int64_t	flags,
		blocks,		/* Total blocks. */
		bfree,		/* Free blocks available to superuser. */
		bavail,		/* Free blocks available to non-superuser. */
		files,		/* Total file nodes. */
		ffree;		/* Free file nodes. */
};

#define glibtop_get_fsusage(fsusage,disk)	glibtop_get_fsusage_l(glibtop_global_server, fsusage, disk)

#define glibtop_get_fsusage_r		glibtop_get_fsusage_s

void glibtop_get_fsusage_l (glibtop *server, glibtop_fsusage *buf, const char *mount_dir);
void glibtop_get_fsusage_s (glibtop *server, glibtop_fsusage *buf, const char *mount_dir);

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_fsusage [];
extern const unsigned glibtop_types_fsusage [];
extern const char *glibtop_labels_fsusage [];
extern const char *glibtop_descriptions_fsusage [];

#endif

END_LIBGTOP_DECLS

#endif
