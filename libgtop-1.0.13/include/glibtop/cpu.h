/* $Id: cpu.h,v 1.14 1999/02/23 11:42:50 martin Exp $ */

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

#ifndef __GLIBTOP_CPU_H__
#define __GLIBTOP_CPU_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_CPU_TOTAL	0
#define GLIBTOP_CPU_USER	1
#define GLIBTOP_CPU_NICE	2
#define GLIBTOP_CPU_SYS		3
#define GLIBTOP_CPU_IDLE	4
#define GLIBTOP_CPU_FREQUENCY	5

#define GLIBTOP_XCPU_TOTAL	6
#define GLIBTOP_XCPU_USER	7
#define GLIBTOP_XCPU_NICE	8
#define GLIBTOP_XCPU_SYS	9
#define GLIBTOP_XCPU_IDLE	10

#define GLIBTOP_MAX_CPU		11

/* Nobody should really be using more than 4 processors. */
#define GLIBTOP_NCPU		4

typedef struct _glibtop_cpu	glibtop_cpu;

struct _glibtop_cpu
{
	u_int64_t	flags,
		total,				/* GLIBTOP_CPU_TOTAL		*/
		user,				/* GLIBTOP_CPU_USER		*/
		nice,				/* GLIBTOP_CPU_NICE		*/
		sys,				/* GLIBTOP_CPU_SYS		*/
		idle,				/* GLIBTOP_CPU_IDLE		*/
		frequency,			/* GLIBTOP_CPU_FREQUENCY	*/
		xcpu_total [GLIBTOP_NCPU],	/* GLIBTOP_XCPU_TOTAL		*/
		xcpu_user [GLIBTOP_NCPU],	/* GLIBTOP_XCPU_USER		*/
		xcpu_nice [GLIBTOP_NCPU],	/* GLIBTOP_XCPU_NICE		*/
		xcpu_sys  [GLIBTOP_NCPU],	/* GLIBTOP_XCPU_SYS		*/
		xcpu_idle [GLIBTOP_NCPU];	/* GLIBTOP_XCPU_IDLE		*/
};

#define glibtop_get_cpu(cpu)	glibtop_get_cpu_l(glibtop_global_server, cpu)

#if GLIBTOP_SUID_CPU
#define glibtop_get_cpu_r	glibtop_get_cpu_p
#else
#define glibtop_get_cpu_r	glibtop_get_cpu_s
#endif

void glibtop_get_cpu_l (glibtop *server, glibtop_cpu *buf);

#if GLIBTOP_SUID_CPU
void glibtop_init_cpu_p (glibtop *server);
void glibtop_get_cpu_p (glibtop *server, glibtop_cpu *buf);
#else
void glibtop_init_cpu_s (glibtop *server);
void glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_cpu [];
extern const unsigned glibtop_types_cpu [];
extern const char *glibtop_labels_cpu [];
extern const char *glibtop_descriptions_cpu [];

#endif

END_LIBGTOP_DECLS

#endif
