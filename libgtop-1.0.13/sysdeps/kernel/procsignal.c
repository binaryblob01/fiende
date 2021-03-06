/* $Id: procsignal.c,v 1.9 1999/02/23 11:44:42 martin Exp $ */

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
#include <glibtop/procsignal.h>

#include "kernel.h"

static const unsigned long _glibtop_sysdeps_proc_signal =
(1 << GLIBTOP_PROC_SIGNAL_SIGNAL) + (1 << GLIBTOP_PROC_SIGNAL_BLOCKED) +
(1 << GLIBTOP_PROC_SIGNAL_SIGIGNORE) + (1 << GLIBTOP_PROC_SIGNAL_SIGCATCH);

/* Init function. */

void
glibtop_init_proc_signal_s (glibtop *server)
{
	server->sysdeps.proc_signal = _glibtop_sysdeps_proc_signal;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_signal_s (glibtop *server, glibtop_proc_signal *buf,
			   pid_t pid)
{
	union table tbl;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_SIGNAL, 0);

	memset (buf, 0, sizeof (glibtop_proc_signal));

	if (table (TABLE_PROC_SIGNAL, &tbl, &pid))
		glibtop_error_io_r (server, "table(TABLE_PROC_SIGNAL)");

	buf->flags = _glibtop_sysdeps_proc_signal;

	buf->signal [0] = tbl.proc_signal.signal;
	buf->blocked [0] = tbl.proc_signal.blocked;
	buf->sigignore [0] = tbl.proc_signal.ignored;
	buf->sigcatch [0] = tbl.proc_signal.caught;
}
