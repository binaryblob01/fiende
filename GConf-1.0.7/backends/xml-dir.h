/* GConf
 * Copyright (C) 1999, 2000 Red Hat Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef GCONF_XML_DIR_H
#define GCONF_XML_DIR_H

#include <gconf/gconf.h>
#include <libxml/tree.h>

/* Dir stores the information about a given directory */

typedef struct _Dir Dir;
Dir*           dir_new             (const gchar  *keyname,
                                    const gchar  *xml_root_dir,
                                    guint dir_mode,
                                    guint file_mode);
Dir*           dir_load            (const gchar  *key,
                                    const gchar  *xml_root_dir,
                                    GError  **err);
void           dir_destroy         (Dir          *d);
void           dir_clear_cache     (Dir          *d);
gboolean       dir_ensure_exists   (Dir          *d,
                                    GError  **err);
gboolean       dir_sync            (Dir          *d,
                                    GError  **err);

const gchar*   dir_get_name        (Dir          *d);

/* key should have no slashes in it */
void           dir_set_value       (Dir          *d,
                                    const gchar  *relative_key,
                                    GConfValue   *value,
                                    GError  **err);
GConfValue*    dir_get_value       (Dir          *d,
                                    const gchar  *relative_key,
                                    const gchar **locales,
                                    gchar       **schema_name,
                                    GError  **err);
GConfMetaInfo* dir_get_metainfo    (Dir          *d,
                                    const gchar  *relative_key,
                                    GError  **err);
void           dir_unset_value     (Dir          *d,
                                    const gchar  *relative_key,
                                    const gchar  *locale,
                                    GError  **err);
GSList*        dir_all_entries     (Dir          *d,
                                    const gchar **locales,
                                    GError  **err);
GSList*        dir_all_subdirs     (Dir          *d,
                                    GError  **err);
void           dir_set_schema      (Dir          *d,
                                    const gchar  *relative_key,
                                    const gchar  *schema_key,
                                    GError  **err);
GTime          dir_get_last_access (Dir          *d);

gboolean       dir_sync_pending    (Dir          *d);

/* Marks for deletion; dir cache really has to implement directory
   deletion, since it is recursive */
void           dir_mark_deleted    (Dir          *d);
gboolean       dir_is_deleted      (Dir          *d);


/* random utility function */
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
guint mode_t_to_mode(mode_t orig);

#endif
