/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/* 
 * Copyright (C) 2000 Eazel, Inc
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Maciej Stachowiak
 */

/* main.c - main function and object activation function for loser
   content view component that fails on command. */

#include <config.h>

#include "nautilus-content-loser.h"

#include <gnome.h>
#include <liboaf/liboaf.h>
#include <bonobo.h>

static int object_count = 0;

static void
loser_object_destroyed(GtkObject *obj)
{
	object_count--;
	if (object_count <= 0) {
		gtk_main_quit ();
	}
}

static BonoboObject *
loser_make_object (BonoboGenericFactory *factory, 
		    const char *iid, 
		    void *closure)
{
	NautilusContentLoser *view;
	NautilusView *nautilus_view;

	nautilus_content_loser_maybe_fail ("pre-make-object");

	if (strcmp (iid, "OAFIID:nautilus_content_loser:95901458-c68b-43aa-aaca-870ced11062d")) {
		return NULL;
	}

	view = NAUTILUS_CONTENT_LOSER (gtk_object_new (NAUTILUS_TYPE_CONTENT_LOSER, NULL));

	object_count++;

	nautilus_view = nautilus_content_loser_get_nautilus_view (view);

	gtk_signal_connect (GTK_OBJECT (nautilus_view), "destroy", loser_object_destroyed, NULL);

	nautilus_content_loser_maybe_fail ("post-make-object");

	return BONOBO_OBJECT (nautilus_view);
}

int main(int argc, char *argv[])
{
	BonoboGenericFactory *factory;
	CORBA_ORB orb;
	CORBA_Environment ev;
	char *registration_id;

	CORBA_exception_init(&ev);

	nautilus_content_loser_maybe_fail ("pre-init");

	gnomelib_register_popt_table (oaf_popt_options, oaf_get_popt_table_name ());
	orb = oaf_init (argc, argv);

        gnome_init ("nautilus-content-loser", VERSION, 
		    argc, argv); 

	bonobo_init (orb, CORBA_OBJECT_NIL, CORBA_OBJECT_NIL);

	nautilus_content_loser_maybe_fail ("post-init");

        registration_id = oaf_make_registration_id ("OAFIID:nautilus_content_loser_factory:adf30e75-3b63-4360-8784-a8e239390a69", getenv ("DISPLAY"));
	factory = bonobo_generic_factory_new_multi (registration_id, 
						    loser_make_object,
						    NULL);
	g_free (registration_id);

		
	nautilus_content_loser_maybe_fail ("post-factory-init");

	do {
		bonobo_main ();
	} while (object_count > 0);
	
	return 0;
}
