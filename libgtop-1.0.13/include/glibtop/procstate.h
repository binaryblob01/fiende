/* $Id: procstate.h,v 1.13 1999/02/23 11:43:11 martin Exp $ */

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

#ifndef __GLIBTOP_PROCSTATE_H__
#define __GLIBTOP_PROCSTATE_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_PROC_STATE_CMD		0
#define GLIBTOP_PROC_STATE_STATE	1
#define GLIBTOP_PROC_STATE_UID		2
#define GLIBTOP_PROC_STATE_GID		3

#define GLIBTOP_MAX_PROC_STATE		4

typedef struct _glibtop_proc_state	glibtop_proc_state;

/* Name and status */

struct _glibtop_proc_state
{
	u_int64_t flags;
	char cmd[40],		/* basename of executable file in 
				 * call to exec(2) */
		state;		/* single-char code for process state
				 * (S=sleeping) */
	/* NOTE: when porting the library, TRY HARD to implement the
	 *       following two fields. */
	/* IMPORTANT NOTICE: For security reasons, it is extremely important
	 *                   only to set the flags value for those two
	 *                   fields if their values are corrent ! */
	int uid,		/* UID of process */
		gid;		/* GID of process */
};

#define glibtop_get_proc_state(p1, p2)	glibtop_get_proc_state_l(glibtop_global_server, p1, p2)

#if GLIBTOP_SUID_PROC_STATE
#define glibtop_get_proc_state_r	glibtop_get_proc_state_p
#else
#define glibtop_get_proc_state_r	glibtop_get_proc_state_s
#endif

void glibtop_get_proc_state_l (glibtop *server, glibtop_proc_state *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_STATE
void glibtop_init_proc_state_p (glibtop *server);
void glibtop_get_proc_state_p (glibtop *server, glibtop_proc_state *buf, pid_t pid);
#else
void glibtop_init_proc_state_s (glibtop *server);
void glibtop_get_proc_state_s (glibtop *server, glibtop_proc_state *buf, pid_t pid);
#endif
     
#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_state [];
extern const unsigned glibtop_types_proc_state [];
extern const char *glibtop_labels_proc_state [];
extern const char *glibtop_descriptions_proc_state [];

#endif

END_LIBGTOP_DECLS

#endif
