/* $Id: procuid.h,v 1.13 1999/02/23 11:43:13 martin Exp $ */

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

#ifndef __GLIBTOP_PROCUID_H__
#define __GLIBTOP_PROCUID_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_PROC_UID_UID		0
#define GLIBTOP_PROC_UID_EUID		1
#define GLIBTOP_PROC_UID_GID		2
#define GLIBTOP_PROC_UID_EGID		3
#define GLIBTOP_PROC_UID_PID		4
#define GLIBTOP_PROC_UID_PPID		5
#define GLIBTOP_PROC_UID_PGRP		6
#define GLIBTOP_PROC_UID_SESSION	7
#define GLIBTOP_PROC_UID_TTY		8
#define GLIBTOP_PROC_UID_TPGID		9
#define GLIBTOP_PROC_UID_PRIORITY	10
#define GLIBTOP_PROC_UID_NICE		11

#define GLIBTOP_MAX_PROC_UID		12

typedef struct _glibtop_proc_uid	glibtop_proc_uid;

/* User, Group, Terminal and session */

struct _glibtop_proc_uid
{
	u_int64_t flags;
	int uid,		/* user id */
		euid,		/* effective user id */
		gid,		/* group id */
		egid,		/* effective group id */
		pid,		/* process id */
		ppid,		/* pid of parent process */
		pgrp,		/* process group id */
		session,	/* session id */
		tty,		/* full device number of controlling terminal */
		tpgid,		/* terminal process group id */
		priority,	/* kernel scheduling priority */
		nice;		/* standard unix nice level of process */
};

#define glibtop_get_proc_uid(p1, p2)	glibtop_get_proc_uid_l(glibtop_global_server, p1, p2)

#if GLIBTOP_SUID_PROC_UID
#define glibtop_get_proc_uid_r		glibtop_get_proc_uid_p
#else
#define glibtop_get_proc_uid_r		glibtop_get_proc_uid_s
#endif

void glibtop_get_proc_uid_l (glibtop *server, glibtop_proc_uid *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_UID
void glibtop_init_proc_uid_p (glibtop *server);
void glibtop_get_proc_uid_p (glibtop *server, glibtop_proc_uid *buf, pid_t pid);
#else
void glibtop_init_proc_uid_s (glibtop *server);
void glibtop_get_proc_uid_s (glibtop *server, glibtop_proc_uid *buf, pid_t pid);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_uid [];
extern const unsigned glibtop_types_proc_uid [];
extern const char *glibtop_labels_proc_uid [];
extern const char *glibtop_descriptions_proc_uid [];

#endif

END_LIBGTOP_DECLS

#endif
