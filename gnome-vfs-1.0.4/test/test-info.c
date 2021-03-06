/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/* test-info.c - Test program for the `get_file_info()' functionality of the
   GNOME Virtual File System.

   Copyright (C) 1999 Free Software Foundation

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.

   Author: Ettore Perazzoli <ettore@comm2000.it> */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gnome.h>

#include "gnome-vfs.h"

static const gchar *
type_to_string (GnomeVFSFileType type)
{
	switch (type) {
	case GNOME_VFS_FILE_TYPE_UNKNOWN:
		return "Unknown";
	case GNOME_VFS_FILE_TYPE_REGULAR:
		return "Regular";
	case GNOME_VFS_FILE_TYPE_DIRECTORY:
		return "Directory";
	case GNOME_VFS_FILE_TYPE_SYMBOLIC_LINK:
		return "Symbolic Link";
	case GNOME_VFS_FILE_TYPE_FIFO:
		return "FIFO";
	case GNOME_VFS_FILE_TYPE_SOCKET:
		return "Socket";
	case GNOME_VFS_FILE_TYPE_CHARACTER_DEVICE:
		return "Character device";
	case GNOME_VFS_FILE_TYPE_BLOCK_DEVICE:
		return "Block device";
	default:
		return "???";
	}
}


static void
print_file_info (const GnomeVFSFileInfo *info)
{
#define FLAG_STRING(info, which)				\
	(GNOME_VFS_FILE_INFO_##which (info) ? "YES" : "NO")

	printf ("Name              : %s\n", info->name);

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_TYPE)
		printf ("Type              : %s\n", type_to_string (info->type));

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_SYMLINK_NAME && info->symlink_name != NULL)
		printf ("Symlink to        : %s\n", info->symlink_name);

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_MIME_TYPE)
		printf ("MIME type         : %s\n", info->mime_type);

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_SIZE)
		printf ("Size              : %" GNOME_VFS_SIZE_FORMAT_STR "\n",
			info->size);

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_BLOCK_COUNT)
		printf ("Blocks            : %" GNOME_VFS_SIZE_FORMAT_STR "\n",
			info->block_count);

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_IO_BLOCK_SIZE)
		printf ("I/O block size    : %d\n", info->io_block_size);

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_FLAGS) {
		printf ("Local             : %s\n", FLAG_STRING (info, LOCAL));
		printf ("SUID              : %s\n", FLAG_STRING (info, SUID));
		printf ("SGID              : %s\n", FLAG_STRING (info, SGID));
		printf ("Sticky            : %s\n", FLAG_STRING (info, STICKY));
	}

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_PERMISSIONS)
		printf ("Permissions       : %04o\n", info->permissions);

	
	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_LINK_COUNT)
		printf ("Link count        : %d\n", info->link_count);
	
	printf ("UID               : %d\n", info->uid);
	printf ("GID               : %d\n", info->gid);

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_ATIME)
		printf ("Access time       : %s", ctime (&info->atime));

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_MTIME)
		printf ("Modification time : %s", ctime (&info->mtime));

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_CTIME)
		printf ("Change time       : %s", ctime (&info->ctime));

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_DEVICE)
		printf ("Device #          : %ld\n", (gulong) info->device);

	if(info->valid_fields&GNOME_VFS_FILE_INFO_FIELDS_INODE)
		printf ("Inode #           : %ld\n", (gulong) info->inode);


#undef FLAG_STRING
}

int
main (int argc,
      char **argv)
{
	GnomeVFSFileInfo *info;
	GnomeVFSResult result;
	gchar *uri;
	int i=1;

	if (argc < 2) {
		fprintf (stderr, "Usage: %s <uri> [<uri>...]\n", argv[0]);
		return 1;
	}

	if (!gnome_vfs_init ()) {
		fprintf (stderr, "%s: Cannot initialize the GNOME Virtual File System.\n",
			 argv[0]);
		return 1;
	}

	while (i < argc) {

		uri = argv[i];

		g_print("Getting info for \"%s\".\n", uri);

		info = gnome_vfs_file_info_new ();
		result = gnome_vfs_get_file_info (uri, 
						  info,
						  (GNOME_VFS_FILE_INFO_GET_MIME_TYPE
						   | GNOME_VFS_FILE_INFO_FOLLOW_LINKS));
		if (result != GNOME_VFS_OK) {
			fprintf (stderr, "%s: %s: %s\n",
				 argv[0], uri, gnome_vfs_result_to_string (result));
			i++;
			return result;
		}

		print_file_info (info);

		gnome_vfs_file_info_unref (info);

		i++;
	}

	return 0;
}
