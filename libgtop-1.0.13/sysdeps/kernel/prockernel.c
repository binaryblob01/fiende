/* $Id: prockernel.c,v 1.8 1999/02/23 11:44:37 martin Exp $ */

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
#include <glibtop/prockernel.h>

#include "kernel.h"

static const unsigned long _glibtop_sysdeps_proc_kernel =
(1 << GLIBTOP_PROC_KERNEL_K_FLAGS) + (1 << GLIBTOP_PROC_KERNEL_MIN_FLT) +
(1 << GLIBTOP_PROC_KERNEL_MAJ_FLT) + (1 << GLIBTOP_PROC_KERNEL_CMIN_FLT) +
(1 << GLIBTOP_PROC_KERNEL_CMAJ_FLT) + (1 << GLIBTOP_PROC_KERNEL_KSTK_ESP) +
(1 << GLIBTOP_PROC_KERNEL_KSTK_EIP) + (1 << GLIBTOP_PROC_KERNEL_NWCHAN);

/* Init function. */

void
glibtop_init_proc_kernel_s (glibtop *server)
{
	server->sysdeps.proc_kernel = _glibtop_sysdeps_proc_kernel;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_kernel_s (glibtop *server, glibtop_proc_kernel *buf,
			   pid_t pid)
{
	union table tbl;
	
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_KERNEL, 0);
	
	memset (buf, 0, sizeof (glibtop_proc_kernel));
	
	if (table (TABLE_PROC_KERNEL, &tbl, &pid))
		glibtop_error_io_r (server, "table(TABLE_PROC_KERNEL)");
	
	buf->flags = _glibtop_sysdeps_proc_kernel;
	
	buf->min_flt = tbl.proc_kernel.min_flt;
	buf->cmin_flt = tbl.proc_kernel.cmin_flt;
	buf->maj_flt = tbl.proc_kernel.maj_flt;
	buf->cmaj_flt = tbl.proc_kernel.cmaj_flt;
	buf->kstk_esp = tbl.proc_kernel.kesp;
	buf->kstk_eip = tbl.proc_kernel.keip;
	buf->nwchan = tbl.proc_kernel.wchan;
}
