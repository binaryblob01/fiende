/* $Id: cpu.c,v 1.7 1999/02/23 11:44:26 martin Exp $ */

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

#include <config.h>
#include <glibtop/error.h>
#include <glibtop/cpu.h>

#include "kernel.h"

static const unsigned long _glibtop_sysdeps_cpu =
(1 << GLIBTOP_CPU_TOTAL) + (1 << GLIBTOP_CPU_USER) +
(1 << GLIBTOP_CPU_NICE) + (1 << GLIBTOP_CPU_SYS) +
(1 << GLIBTOP_CPU_IDLE) + (1 << GLIBTOP_CPU_FREQUENCY);

/* Init function. */

void
glibtop_init_cpu_s (glibtop *server)
{
	server->sysdeps.cpu = _glibtop_sysdeps_cpu;
}

/* Provides information about cpu usage. */

void
glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf)
{
	union table tbl;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_CPU, 0);

	memset (buf, 0, sizeof (glibtop_cpu));

	if (table (TABLE_CPU, &tbl, NULL))
		glibtop_error_io_r (server, "table(TABLE_CPU)");

	buf->flags = _glibtop_sysdeps_cpu;

	buf->total = tbl.cpu.total;
	buf->user = tbl.cpu.user;
	buf->nice = tbl.cpu.nice;
	buf->sys = tbl.cpu.sys;
	buf->idle = tbl.cpu.idle;
	buf->frequency = tbl.cpu.frequency;
}
