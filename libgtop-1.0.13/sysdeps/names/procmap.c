/* $Id: procmap.c,v 1.3 1999/02/23 11:45:31 martin Exp $ */

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

#include <glibtop/procmap.h>

const char *glibtop_names_proc_map [GLIBTOP_MAX_PROC_MAP] =
{
	"number", "total", "size"
};

const unsigned glibtop_types_proc_map [GLIBTOP_MAX_PROC_MAP] =
{
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_proc_map [GLIBTOP_MAX_PROC_MAP] =
{
	N_("Number of list elements"),
	N_("Total size of list"),
	N_("Size of a single list element")
};

const char *glibtop_descriptions_proc_map [GLIBTOP_MAX_PROC_MAP] =
{
	N_("Number of list elements"),
	N_("Total size of list"),
	N_("Size of a single list element")
};
