/* Copyright (C) 1992, 93, 94, 95, 96, 97 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#if HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#undef PARAMS
#if defined (__GNUC__) || __STDC__
# define PARAMS(args) args
#else
# define PARAMS(args) ()
#endif

#define set_errno(e) (errno = (e))

int
scandir (dir, namelist, select, cmp)
     const char *dir;
     struct dirent ***namelist;
     int (*select) PARAMS ((struct dirent *));
     int (*cmp) PARAMS ((const void *, const void *));
{
  DIR *dp = opendir (dir);
  struct dirent **v = NULL;
  size_t vsize = 0, i;
  struct dirent *d;
  int save;

  if (dp == NULL)
    return -1;

  save = errno;
  set_errno (0);

  i = 0;
  while ((d = readdir (dp)) != NULL)
    if (select == NULL || (*select) (d))
      {
	size_t dsize;

	/* Ignore errors from select or readdir */
	set_errno (0);

	if (i == vsize)
	  {
	    struct dirent **new;
	    if (vsize == 0)
	      vsize = 10;
	    else
	      vsize *= 2;
	    new = (struct dirent **) realloc (v, vsize * sizeof (*v));
	    if (new == NULL)
	      {
	      lose:
		set_errno (ENOMEM);
		break;
	      }
	    v = new;
	  }

	dsize = &d->d_name[1 + NAMLEN (d)] - (char *) d;
	v[i] = (struct dirent *) malloc (dsize);
	if (v[i] == NULL)
	  goto lose;

	memcpy (v[i++], d, dsize);
      }

  if (errno != 0)
    {
      save = errno;
      (void) closedir (dp);
      while (i > 0)
	free (v[--i]);
      free (v);
      set_errno (save);
      return -1;
    }

  (void) closedir (dp);
  set_errno (save);

  /* Sort the list if we have a comparison function to sort with.  */
  if (cmp != NULL)
    qsort (v, i, sizeof (*v), cmp);
  *namelist = v;
  return i;
}

int
alphasort (const void *a, const void *b)
{
  return strcmp ((*(const struct dirent **) a)->d_name,
		 (*(const struct dirent **) b)->d_name);
}
