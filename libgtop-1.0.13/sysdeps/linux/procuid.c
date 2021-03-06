/* $Id: procuid.c,v 1.12.2.1 1999/05/27 19:24:27 martin Exp $ */

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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procuid.h>

static const unsigned long _glibtop_sysdeps_proc_uid =
(1L << GLIBTOP_PROC_UID_UID) + (1L << GLIBTOP_PROC_UID_EUID) +
(1L << GLIBTOP_PROC_UID_GID) + (1L << GLIBTOP_PROC_UID_EGID);

static const unsigned long _glibtop_sysdeps_proc_uid_stat =
(1L << GLIBTOP_PROC_UID_PID) + (1L << GLIBTOP_PROC_UID_PPID) +
(1L << GLIBTOP_PROC_UID_PGRP) + (1L << GLIBTOP_PROC_UID_SESSION) +
(1L << GLIBTOP_PROC_UID_TTY) + (1L << GLIBTOP_PROC_UID_TPGID) +
(1L << GLIBTOP_PROC_UID_PRIORITY) + (1L << GLIBTOP_PROC_UID_NICE);

#define LINUX_VERSION(x,y,z)	(0x10000*(x) + 0x100*(y) + z)

/* Init function. */

void
glibtop_init_proc_uid_s (glibtop *server)
{
	server->sysdeps.proc_uid = _glibtop_sysdeps_proc_uid |
		_glibtop_sysdeps_proc_uid_stat;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_uid_s (glibtop *server, glibtop_proc_uid *buf, pid_t pid)
{
	char buffer [BUFSIZ], *p;
	
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_UID, 0);

	memset (buf, 0, sizeof (glibtop_proc_uid));

	if (proc_status_to_buffer (buffer, pid))
		return;

	/* Search substring 'Pid:' */

	p = strstr (buffer, "\nPid:");
	if (!p) return;

	p = skip_token (p); /* "Pid:" */
	buf->pid = strtoul (p, &p, 0);

	p = skip_token (p); /* "PPid:" */
	buf->ppid = strtoul (p, &p, 0);

	/* Maybe future Linux versions place something between
	 * "PPid" and "Uid", so we catch this here. */
	p = strstr (p, "\nUid:");
	if (!p) return;

	p = skip_token (p); /* "Uid:" */
	buf->uid = strtoul (p, &p, 0);
	buf->euid = strtoul (p, &p, 0);

	/* We don't know how many entries on the "Uid:" line
	 * future Linux version will have, so we catch this here. */
	p = strstr (p, "\nGid:");
	if (!p) return;

	p = skip_token (p); /* "Gid:" */
	buf->gid = strtoul (p, &p, 0);
	buf->egid = strtoul (p, &p, 0);

	buf->flags = _glibtop_sysdeps_proc_uid;

	if (proc_stat_to_buffer (buffer, pid))
		return;

	p = proc_stat_after_cmd (buffer);
	if (!p) return;

	p = skip_multiple_token (p, 2);

	buf->pgrp = strtoul (p, &p, 0);
	buf->session = strtoul (p, &p, 0);
	buf->tty = strtoul (p, &p, 0);
	buf->tpgid = strtoul (p, &p, 0);

	p = skip_multiple_token (p, 9);
	
	buf->priority = strtoul (p, &p, 0);
	buf->nice = strtoul (p, &p, 0);

	if (buf->tty == 0)
		/* the old notty val, update elsewhere bef. moving to 0 */
		buf->tty = -1;

	if (server->os_version_code < LINUX_VERSION(1,3,39)) {
		/* map old meanings to new */
		buf->priority = 2*15 - buf->priority;
		buf->nice = 15 - buf->nice;
	}
	if (server->os_version_code < LINUX_VERSION(1,1,30) && buf->tty != -1)
		/* when tty wasn't full devno */
		buf->tty = 4*0x100 + buf->tty;
	
	buf->flags |= _glibtop_sysdeps_proc_uid_stat;
}
