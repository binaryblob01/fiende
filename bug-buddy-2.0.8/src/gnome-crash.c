/* gnome program that crashes
 *
 * Copyright (C) Jacob Berkman
 *
 * Author: Jacob Berkman  <jberkman@andrew.cmu.edu>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <gnome.h>

int
main (int argc, char *argv[])
{
	int *n = NULL;
	gnome_client_disable_master_connection ();
	gnome_init (PACKAGE, VERSION, argc, argv);
	n[27] = 10-7-78;
	return 0;
}
