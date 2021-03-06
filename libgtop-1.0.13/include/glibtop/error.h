/* $Id: error.h,v 1.11.4.1 1999/09/16 21:12:20 martin Exp $ */

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

#ifndef __GLIBTOP_ERROR_H__
#define __GLIBTOP_ERROR_H__

#include <glibtop.h>

BEGIN_LIBGTOP_DECLS

#ifndef G_GNUC_UNUSED
#if	__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define G_GNUC_UNUSED				\
  __attribute__((unused))
#else	/* !__GNUC__ */
#define	G_GNUC_UNUSED
#endif	/* !__GNUC__ */
#endif /* defined G_GNUC_UNUSED */

void glibtop_error_vr (glibtop *server, char *format, va_list args);
void glibtop_warn_vr (glibtop *server, char *format, va_list args);

void glibtop_error_io_vr (glibtop *server, char *format, int, va_list args);
void glibtop_warn_io_vr (glibtop *server, char *format, int, va_list args);

static void G_GNUC_UNUSED
glibtop_error_r (glibtop *server, char *format, ...)
{
	va_list args;

	va_start (args, format);
	glibtop_error_vr (server, format, args);
	va_end (args);
}

static void G_GNUC_UNUSED
glibtop_warn_r (glibtop *server, char *format, ...)
{
	va_list args;

	va_start (args, format);
	glibtop_warn_vr (server, format, args);
	va_end (args);
}

static void G_GNUC_UNUSED
glibtop_error_io_r (glibtop *server, char *format, ...)
{
	va_list args;

	va_start (args, format);
	glibtop_error_io_vr (server, format, errno, args);
	va_end (args);
}

static void G_GNUC_UNUSED
glibtop_warn_io_r (glibtop *server, char *format, ...)
{
	va_list args;

	va_start (args, format);
	glibtop_warn_io_vr (server, format, errno, args);
	va_end (args);
}

#ifdef  __GNUC__

#define glibtop_error(p1, args...)	glibtop_error_r(glibtop_global_server , p1 , ## args)
#define glibtop_warn(p1, args...)	glibtop_warn_r(glibtop_global_server , p1 , ## args)

#define glibtop_error_io(p1, args...)	glibtop_error_io_r(glibtop_global_server , p1 , ## args)
#define glibtop_warn_io(p1, args...)	glibtop_warn_io_r(glibtop_global_server , p1 , ## args)

#else /* no __GNUC__ */

static void
glibtop_error (char *format, ...)
{
	va_list args;
	va_start (args, format);
	glibtop_error_vr (glibtop_global_server, format, args);
	va_end (args);
}

static void
glibtop_warn (char *format, ...)
{
	va_list args;
	va_start (args, format);
	glibtop_warn_vr (glibtop_global_server, format, args);
	va_end (args);
}

static void
glibtop_error_io (char *format, ...)
{
	va_list args;
	va_start (args, format);
	glibtop_error_io_vr (glibtop_global_server, format, errno, args);
	va_end (args);
}

static void
glibtop_warn_io (char *format, ...)
{
	va_list args;
	va_start (args, format);
	glibtop_warn_io_vr (glibtop_global_server, format, errno, args);
	va_end (args);
}

#endif /* no __GNUC__ */

END_LIBGTOP_DECLS

#endif
