/* $Id: procmem.c,v 1.14.2.1 1999/05/27 19:24:22 martin Exp $ */

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
#include <glibtop/procmem.h>

static const unsigned long _glibtop_sysdeps_proc_mem =
(1L << GLIBTOP_PROC_MEM_VSIZE) + (1L << GLIBTOP_PROC_MEM_RSS) +
(1L << GLIBTOP_PROC_MEM_RSS_RLIM);

static const unsigned long _glibtop_sysdeps_proc_mem_statm =
(1L << GLIBTOP_PROC_MEM_SIZE) + (1L << GLIBTOP_PROC_MEM_RESIDENT) +
(1L << GLIBTOP_PROC_MEM_SHARE);

#ifndef LOG1024
#define LOG1024		10
#endif

/* these are for getting the memory statistics */
static int pageshift;		/* log base 2 of the pagesize */

/* define pagetok in terms of pageshift */
#define pagetok(size) ((size) << pageshift)

/* Init function. */

void
glibtop_init_proc_mem_s (glibtop *server)
{
	register int pagesize;

	server->sysdeps.proc_mem = _glibtop_sysdeps_proc_mem |
	  _glibtop_sysdeps_proc_mem_statm;

	/* get the page size with "getpagesize" and calculate pageshift
	 * from it */
	pagesize = getpagesize ();
	pageshift = 0;
	while (pagesize > 1) {
		pageshift++;
		pagesize >>= 1;
	}
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_mem_s (glibtop *server, glibtop_proc_mem *buf, pid_t pid)
{
	char buffer [BUFSIZ], *p;
	
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_MEM, 0);

	memset (buf, 0, sizeof (glibtop_proc_mem));

	if (proc_stat_to_buffer (buffer, pid))
		return;

	p = proc_stat_after_cmd (buffer);
	if (!p) return;

	p = skip_multiple_token (p, 20);

	buf->vsize = strtoul (p, &p, 0);
	buf->rss = strtoul (p, &p, 0);
	buf->rss_rlim = strtoul (p, &p, 0);

	buf->flags = _glibtop_sysdeps_proc_mem;

	if (proc_statm_to_buffer (buffer, pid))
		return;

	buf->size = strtoul (buffer, &p, 0);
	buf->resident = strtoul (p, &p, 0);
	buf->share = strtoul (p, &p, 0);

	buf->size <<= pageshift;
	buf->resident <<= pageshift;
	buf->share <<= pageshift;
	buf->rss <<= pageshift;

	buf->flags |= _glibtop_sysdeps_proc_mem_statm;
}
