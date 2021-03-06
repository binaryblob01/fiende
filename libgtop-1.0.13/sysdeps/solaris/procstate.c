/* $Id: procstate.c,v 1.2.2.1 2001/02/14 23:07:41 martin Exp $ */

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
#include <glibtop/procstate.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_state =
#if (LIBGTOP_VERSION_CODE >= 1001002) && defined(HAVE_PROCFS_H)
(1L << GLIBTOP_PROC_STATE_HAS_CPU) + (1L << GLIBTOP_PROC_STATE_PROCESSOR) +
(1L << GLIBTOP_PROC_STATE_LAST_PROCESSOR) +
#endif
#if LIBGTOP_VERSION_CODE >= 1001002
(1L << GLIBTOP_PROC_STATE_RUID) + (1L << GLIBTOP_PROC_STATE_RGID) +
#endif
(1L << GLIBTOP_PROC_STATE_CMD) + (1L << GLIBTOP_PROC_STATE_STATE) +
(1L << GLIBTOP_PROC_STATE_UID) + (1L << GLIBTOP_PROC_STATE_GID);

/* Init function. */

void
glibtop_init_proc_state_s (glibtop *server)
{
	server->sysdeps.proc_state = _glibtop_sysdeps_proc_state;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_state_s (glibtop *server, glibtop_proc_state *buf, pid_t pid)
{
#ifdef HAVE_PROCFS_H
	struct psinfo psinfo;
#else
	struct prpsinfo psinfo;
#endif

	memset (buf, 0, sizeof (glibtop_proc_state));

	if (glibtop_get_proc_data_psinfo_s (server, &psinfo, pid))
		return;

	buf->uid = psinfo.pr_euid;
	buf->gid = psinfo.pr_egid;
#if LIBGTOP_VERSION_CODE >= 1001002
	buf->ruid = psinfo.pr_uid;
	buf->rgid = psinfo.pr_gid;
#endif
#ifdef HAVE_PROCFS_H
	switch(psinfo.pr_lwp.pr_state)
#else
        switch(psinfo.pr_state)
#endif
	{
		case SONPROC: 
#if (LIBGTOP_VERSION_CODE >= 1001002) && defined(HAVE_PROCFS_H)
			buf->has_cpu = 1;
			buf->processor = psinfo.pr_lwp.pr_onpro;
#endif
		case SRUN:
#if LIBGTOP_VERSION_CODE >= 1001002
			buf->state = GLIBTOP_PROCESS_RUNNING;
#else
			buf->state = 'R';
#endif
			break;
		case SZOMB:
#if LIBGTOP_VERSION_CODE >= 1001002
			buf->state = GLIBTOP_PROCESS_ZOMBIE;
#else
			buf->state = 'Z';
#endif
			break;
		case SSLEEP:
#if LIBGTOP_VERSION_CODE >= 1001002
			buf->state = GLIBTOP_PROCESS_INTERRUPTIBLE;
#else
			buf->state = 'S';
#endif
			break;
		case SSTOP:
#if LIBGTOP_VERSION_CODE >= 1001002
			buf->state = GLIBTOP_PROCESS_STOPPED;
#else
			buf->state = 'T';
#endif
			break;
		case SIDL:
#if LIBGTOP_VERSION_CODE >= 1001002
			buf->state = GLIBTOP_PROCESS_UNINTERRUPTIBLE;
#else
			buf->state = 'D';
#endif
	}
#if (LIBGTOP_VERSION_CODE >= 1001002) && defined(HAVE_PROCFS_H)
	buf->last_processor = psinfo.pr_lwp.pr_onpro;
#endif
	strncpy (buf->cmd, psinfo.pr_fname, 39);

	buf->flags = _glibtop_sysdeps_proc_state;
}
