/* $Id: proctime.c,v 1.8 1999/02/23 11:44:45 martin Exp $ */

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
#include <glibtop/proctime.h>

#include "kernel.h"

static const unsigned long _glibtop_sysdeps_proc_time =
(1 << GLIBTOP_PROC_TIME_UTIME) + (1 << GLIBTOP_PROC_TIME_CUTIME) +
(1 << GLIBTOP_PROC_TIME_STIME) + (1 << GLIBTOP_PROC_TIME_CSTIME) +
(1 << GLIBTOP_PROC_TIME_RTIME) + (1 << GLIBTOP_PROC_TIME_FREQUENCY) +
(1 << GLIBTOP_PROC_TIME_TIMEOUT) + (1 << GLIBTOP_PROC_TIME_IT_REAL_VALUE) +
(1 << GLIBTOP_PROC_TIME_START_TIME);

/* Init function. */

void
glibtop_init_proc_time_s (glibtop *server)
{
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_s (glibtop *server, glibtop_proc_time *buf,
			 pid_t pid)
{
	union table tbl;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_TIME, 0);

	memset (buf, 0, sizeof (glibtop_proc_time));

	if (table (TABLE_PROC_TIME, &tbl, &pid))
	  glibtop_error_io_r (server, "table(TABLE_PROC_TIME)");

	buf->utime = tbl.proc_time.utime;
	buf->stime = tbl.proc_time.stime;

	buf->rtime = buf->utime + buf->stime;

	buf->cutime = tbl.proc_time.cutime;
	buf->cstime = tbl.proc_time.cstime;

	buf->frequency = 100;

	buf->timeout = tbl.proc_time.timeout;
	buf->it_real_value = tbl.proc_time.it_real_value;
	buf->start_time = tbl.proc_time.start_time;
}
