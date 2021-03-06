/* $Id: mem.h,v 1.14 1999/02/23 11:42:56 martin Exp $ */

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

#ifndef __GLIBTOP_MEM_H__
#define __GLIBTOP_MEM_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_MEM_TOTAL	0
#define GLIBTOP_MEM_USED	1
#define GLIBTOP_MEM_FREE	2
#define GLIBTOP_MEM_SHARED	3
#define GLIBTOP_MEM_BUFFER	4
#define GLIBTOP_MEM_CACHED	5
#define GLIBTOP_MEM_USER	6
#define GLIBTOP_MEM_LOCKED	7

#define GLIBTOP_MAX_MEM		8

typedef struct _glibtop_mem	glibtop_mem;

struct _glibtop_mem
{
	u_int64_t	flags,
		total,		/* GLIBTOP_MEM_TOTAL	*/
		used,		/* GLIBTOP_MEM_USED	*/
		free,		/* GLIBTOP_MEM_FREE	*/
		shared,		/* GLIBTOP_MEM_SHARED	*/
		buffer,		/* GLIBTOP_MEM_BUFFER	*/
		cached,		/* GLIBTOP_MEM_CACHED	*/
		user,		/* GLIBTOP_MEM_USER	*/
		locked;		/* GLIBTOP_MEM_LOCKED	*/
};

#define glibtop_get_mem(mem)	glibtop_get_mem_l(glibtop_global_server, mem)

#if GLIBTOP_SUID_MEM
#define glibtop_get_mem_r		glibtop_get_mem_p
#else
#define glibtop_get_mem_r		glibtop_get_mem_s
#endif

void glibtop_get_mem_l (glibtop *server, glibtop_mem *buf);

#if GLIBTOP_SUID_MEM
void glibtop_init_mem_p (glibtop *server);
void glibtop_get_mem_p (glibtop *server, glibtop_mem *buf);
#else
void glibtop_init_mem_s (glibtop *server);
void glibtop_get_mem_s (glibtop *server, glibtop_mem *buf);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_mem [];
extern const unsigned glibtop_types_mem [];
extern const char *glibtop_labels_mem [];
extern const char *glibtop_descriptions_mem [];

#endif

END_LIBGTOP_DECLS

#endif
