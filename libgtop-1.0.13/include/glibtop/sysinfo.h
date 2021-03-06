/* $Id: sysinfo.h,v 1.5 1999/02/23 11:43:21 martin Exp $ */

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

#ifndef __GLIBTOP_SYSINFO_H__
#define __GLIBTOP_SYSINFO_H__

#include <glibtop.h>
#include <glibtop/cpu.h>
#include <glibtop/global.h>

#include <glib.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_SYSINFO_CPUINFO		0

#define GLIBTOP_MAX_SYSINFO		1

typedef struct _glibtop_sysinfo	glibtop_sysinfo;

typedef struct _glibtop_entry	glibtop_entry;

struct _glibtop_entry
{
	GPtrArray	*labels;
	GHashTable	*values;
	GHashTable	*descriptions;
};

struct _glibtop_sysinfo
{
	u_int64_t	flags, ncpu;
	glibtop_entry	cpuinfo [GLIBTOP_NCPU];
};

#define glibtop_get_sysinfo()	glibtop_get_sysinfo_s(glibtop_global_server)

#define glibtop_get_sysinfo_r	glibtop_get_sysinfo_s

glibtop_sysinfo *glibtop_get_sysinfo_s (glibtop *server);

END_LIBGTOP_DECLS

#endif
