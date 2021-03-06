/* $Id: read.c,v 1.9.2.1 1999/05/06 22:09:24 martin Exp $ */

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

#include <glibtop/read.h>

/* Reads some data from server. */

static void
do_read (int s, void *ptr, size_t total_size)
{
	int nread;
	size_t already_read = 0, remaining = total_size;
	char *tmp_ptr;
	
	while (already_read < total_size) {
		nread = recv (s, ptr, remaining, 0);

		if (nread == 0) {
			close (s);
			continue;
		}
		
		if (nread <= 0) {
			glibtop_error_io ("recv");
			return;
		}
		
		already_read += nread;
		remaining -= nread;
		/* (char *) ptr += nread; */
		tmp_ptr = ptr;
		tmp_ptr += nread;
		ptr = tmp_ptr;
	}
}

void
glibtop_read_l (glibtop *server, size_t size, void *buf)
{
	int ret = 0;

	glibtop_init_r (&server, 0, 0);

#ifdef DEBUG
	fprintf (stderr, "LIBRARY: really reading %d bytes.\n", size);
#endif

	if (server->socket) {
		do_read (server->socket, buf, size);
	} else {
		ret = read (server->input [0], buf, size);
	}

	if (ret < 0)
		glibtop_error_io_r (server, _("read %d bytes"), size);
}
