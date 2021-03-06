/* $Id: procstate.c,v 1.12.4.4 1999/10/16 18:01:39 martin Exp $ */

/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <glibtop/procstate.h>

#include <glibtop_suid.h>

#if !defined(__OpenBSD__) 
//&& (!defined __bsdi__)
#include <sys/user.h>
#endif

static const unsigned long _glibtop_sysdeps_proc_state =
(1L << GLIBTOP_PROC_STATE_CMD) + (1L << GLIBTOP_PROC_STATE_UID) +
(1L << GLIBTOP_PROC_STATE_GID);

static const unsigned long _glibtop_sysdeps_proc_state_new =
#if LIBGTOP_VERSION_CODE >= 1001000
(1L << GLIBTOP_PROC_STATE_RUID) + (1L << GLIBTOP_PROC_STATE_RGID);
#else
0;
#endif

/* Init function. */

void
glibtop_init_proc_state_p (glibtop *server)
{
	server->sysdeps.proc_state = _glibtop_sysdeps_proc_state |
		_glibtop_sysdeps_proc_state_new;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_state_p (glibtop *server,
			  glibtop_proc_state *buf,
			  pid_t pid)
{
	struct kinfo_proc *pinfo;
	int count = 0;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_PROC_STATE), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_state));

	/* It does not work for the swapper task. */
	if (pid == 0) return;
	
	/* Get the process information */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

	strncpy (buf->cmd, pinfo [0].kp_proc.p_comm, sizeof (buf->cmd)-1);
	buf->cmd [sizeof (buf->cmd)-1] = 0;

	buf->uid = pinfo [0].kp_eproc.e_pcred.p_svuid;
	buf->gid = pinfo [0].kp_eproc.e_pcred.p_svgid;

#if LIBGTOP_VERSION_CODE >= 1001000
	buf->ruid = pinfo [0].kp_eproc.e_pcred.p_ruid;
	buf->rgid = pinfo [0].kp_eproc.e_pcred.p_rgid;
#endif

	/* Set the flags for the data we're about to return*/
	buf->flags = _glibtop_sysdeps_proc_state |
		_glibtop_sysdeps_proc_state_new;

#if LIBGTOP_VERSION_CODE >= 1001000
	switch (pinfo [0].kp_proc.p_stat) {
	case SIDL:
		buf->state = 0;
		break;
	case SRUN:
		buf->state = GLIBTOP_PROCESS_RUNNING;
		break;
	case SSLEEP:
		buf->state = GLIBTOP_PROCESS_INTERRUPTIBLE;
		break;
	case SSTOP:
		buf->state = GLIBTOP_PROCESS_STOPPED;
		break;
	case SZOMB:
		buf->state = GLIBTOP_PROCESS_ZOMBIE;
		break;
	default:
		return;
	}
#else
	switch (pinfo [0].kp_proc.p_stat) {
	case SIDL:
		buf->state = 'S';
		break;
	case SRUN:
		buf->state = 'R';
		break;
	case SSLEEP:
		buf->state = 'S';
		break;
	case SSTOP:
		buf->state = 'T';
		break;
	case SZOMB:
		buf->state = 'Z';
		break;
	default:
		return;
	}
#endif

	buf->flags |= (1L << GLIBTOP_PROC_STATE_STATE);
}
