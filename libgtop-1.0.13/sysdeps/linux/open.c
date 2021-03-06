/* $Id: open.c,v 1.13.2.1 2001/02/13 12:31:57 martin Exp $ */

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
#include <glibtop/cpu.h>
#include <glibtop/open.h>

/* =====================================================
 * Linux kernel version information for procps utilities
 * Copyright (c) 1996 Charles Blake <cblake@bbn.com>
 */
#include <sys/utsname.h>

#define LINUX_VERSION(x,y,z)   (0x10000*(x) + 0x100*(y) + z)

static int linux_version_code = 0;

static void set_linux_version(void) {
    static struct utsname uts;
    int x = 0, y = 0, z = 0;	/* cleared in case sscanf() < 3 */
    
    if (linux_version_code) return;
    if (uname(&uts) == -1)	/* failure most likely implies impending death */
	exit(1);
    if (sscanf(uts.release, "%d.%d.%d", &x, &y, &z) < 3)
	fprintf(stderr,		/* *very* unlikely to happen by accident */
		"Non-standard uts for running kernel:\n"
		"release %s=%d.%d.%d gives version code %d\n",
		uts.release, x, y, z, LINUX_VERSION(x,y,z));
    linux_version_code = LINUX_VERSION(x, y, z);
}

/* ======================================================= */

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

#define FILENAME	"/proc/stat"

void
glibtop_open_s (glibtop *server, const char *program_name,
		const unsigned long features,
		const unsigned flags)
{
	char buffer [BUFSIZ], *p;
	struct stat statb;
	int fd, len, i;

	server->name = program_name;

	set_linux_version ();
	server->os_version_code = (unsigned long) linux_version_code;

	server->ncpu = 0;

	/* On Linux 2.4.x, /proc/stat has "cpu" and "cpu0" entries even
	 * for non-SMP systems. Checking whether /proc/<pid>/cpu exists
	 * is a much better way to detect SMP. */

	if (stat ("/proc/1/cpu", &statb))
	    return;

	fd = open (FILENAME, O_RDONLY);
	if (fd < 0)
		glibtop_error_io_r (server, "open (%s)", FILENAME);

	len = read (fd, buffer, BUFSIZ-1);
	if (len < 0)
		glibtop_error_io_r (server, "read (%s)", FILENAME);

	close (fd);

	buffer [len] = '\0';

	p = skip_multiple_token (buffer, 5) + 1;

	for (i = 0; i < GLIBTOP_NCPU; i++) {
		
		if (strncmp (p, "cpu", 3) || !isdigit (p [3]))
			break;

		server->ncpu++;

		p = skip_multiple_token (p, 5) + 1;
	}

#if DEBUG	
	printf ("\nThis machine has %d CPUs.\n\n", server->ncpu);
#endif
}
