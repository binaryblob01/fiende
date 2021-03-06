/* $Id: fsusage.c,v 1.3 1999/02/23 11:45:21 martin Exp $ */

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

#include <glibtop/fsusage.h>

const char *glibtop_names_fsusage [GLIBTOP_MAX_FSUSAGE] =
{
	"blocks", "bfree", "bavail", "files", "ffree"
};

const unsigned glibtop_types_fsusage [GLIBTOP_MAX_FSUSAGE] =
{ 
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_fsusage [GLIBTOP_MAX_FSUSAGE] =
{
	N_("Total blocks"),
	N_("Free blocks"),
	N_("Available blocks"),
	N_("Total file nodes"),
	N_("Free file nodes")
};

const char *glibtop_descriptions_fsusage [GLIBTOP_MAX_FSUSAGE] =
{
	N_("Total blocks"),
	N_("Free blocks available to the superuser"),
	N_("Free blocks available to non-superusers"),
	N_("Total file nodes"),
	N_("Free file nodes")
};
