/* $Id: glibtop_server.h,v 1.1.1.1.2.1 2001/02/14 23:07:41 martin Exp $ */

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

#ifndef __GLIBTOP_SERVER_H__
#define __GLIBTOP_SERVER_H__

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_SUID_CPU		0
#define GLIBTOP_SUID_MEM		0
#define GLIBTOP_SUID_SWAP		0
#define GLIBTOP_SUID_UPTIME		0
#define GLIBTOP_SUID_LOADAVG		0
#define GLIBTOP_SUID_SHM_LIMITS		(1L << GLIBTOP_SYSDEPS_SHM_LIMITS)
#define GLIBTOP_SUID_MSG_LIMITS		(1L << GLIBTOP_SYSDEPS_MSG_LIMITS)
#define GLIBTOP_SUID_SEM_LIMITS		(1L << GLIBTOP_SYSDEPS_SEM_LIMITS)
#define GLIBTOP_SUID_PROCLIST		0
#define GLIBTOP_SUID_PROC_STATE		0
#define GLIBTOP_SUID_PROC_UID		0
#define GLIBTOP_SUID_PROC_MEM		0
#define GLIBTOP_SUID_PROC_TIME		0
#define GLIBTOP_SUID_PROC_SIGNAL	0
#define GLIBTOP_SUID_PROC_KERNEL	0
#define GLIBTOP_SUID_PROC_SEGMENT	0
#define GLIBTOP_SUID_PROC_ARGS		0
#define GLIBTOP_SUID_PROC_MAP		0
#define GLIBTOP_SUID_NETLOAD		0
#define GLIBTOP_SUID_PPP		0

END_LIBGTOP_DECLS

#endif
