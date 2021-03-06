/* $Id: prockernel.c,v 1.13.2.1 1999/05/27 19:24:19 martin Exp $ */

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

static const unsigned long _glibtop_sysdeps_proc_kernel =
(1L << GLIBTOP_PROC_KERNEL_K_FLAGS) + (1L << GLIBTOP_PROC_KERNEL_MIN_FLT) +
(1L << GLIBTOP_PROC_KERNEL_MAJ_FLT) + (1L << GLIBTOP_PROC_KERNEL_CMIN_FLT) +
(1L << GLIBTOP_PROC_KERNEL_CMAJ_FLT) + (1L << GLIBTOP_PROC_KERNEL_KSTK_ESP) +
(1L << GLIBTOP_PROC_KERNEL_KSTK_EIP) + (1L << GLIBTOP_PROC_KERNEL_WCHAN);

/* Init function. */

void
glibtop_init_proc_kernel_s (glibtop *server)
{
	server->sysdeps.proc_kernel = _glibtop_sysdeps_proc_kernel;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_kernel_s (glibtop *server, glibtop_proc_kernel *buf, pid_t pid)
{
	char buffer [BUFSIZ], *p;
	
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_KERNEL, 0);

	memset (buf, 0, sizeof (glibtop_proc_kernel));

	if (proc_stat_to_buffer (buffer, pid))
		return;

	p = proc_stat_after_cmd (buffer);
	if (!p) return;

	p = skip_multiple_token (p, 6);

	buf->k_flags   = strtoul (p, &p, 0);
	buf->min_flt   = strtoul (p, &p, 0);
	buf->cmin_flt  = strtoul (p, &p, 0);
	buf->maj_flt   = strtoul (p, &p, 0);
	buf->cmaj_flt  = strtoul (p, &p, 0);

	p = skip_multiple_token (p, 15);

	buf->kstk_esp = strtoul (p, &p, 0);
	buf->kstk_eip = strtoul (p, &p, 0);
	
	p = skip_multiple_token (p, 4);
	
	buf->nwchan = strtoul (p, &p, 0);

	buf->flags = _glibtop_sysdeps_proc_kernel;
}
