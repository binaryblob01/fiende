/* Generated by gen-typeinfo func from "gnome.defs". */

#include <libguile.h>
#include <gtk/gtk.h>

#include <guile-gtk.h>
#include <gnome.h>

typedef void (* set_prop_func)(GnomeClient *client, gint argc, gchar *argv[]);
typedef GnomeClient *new_func (gint argc, gchar *argv[]);

/* Helper function.  */
static void
set_prop_from_arglist (GnomeClient *client, 
		       SCM list, 
		       gchar *name, 
		       set_prop_func func)
{
  char **argv;
  int argc, i;
  SCM l, s;
  
  argc = 0;
  l = list;
  while (SCM_NIMP (l))
    {
      SCM_ASSERT (SCM_CONSP (l), l, SCM_ARGn, name);
      s = SCM_CAR (l);
      SCM_ASSERT (SCM_NIMP (s) && SCM_STRINGP (s), s, SCM_ARGn, name);
      SCM_COERCE_SUBSTR (s);
      ++argc;
      l = SCM_CDR (l);
    }
  SCM_ASSERT (SCM_NULLP (l), list, SCM_ARGn, name);  
  
  SCM_DEFER_INTS;
  argv = (gchar **) malloc (argc * sizeof (gchar *));
  for (i = 0, l = list; SCM_NIMP (l); l = SCM_CDR (l), ++i)
    argv[i] = SCM_CHARS (SCM_CAR (l));
  (*func) (client, argc, argv);
  free (argv);
  SCM_ALLOW_INTS;
}

SCM_PROC (s_gnome_client_set_clone_command, "gnome-client-set-clone-command", 2, 0, 0, sgnome_client_set_clone_command);
SCM
sgnome_client_set_clone_command (SCM p_client, SCM p_list)
{
  GnomeClient* c_client;
  SCM_ASSERT (sgtk_is_a_gtkobj (gnome_client_get_type (), p_client), p_client, SCM_ARG1, s_gnome_client_set_clone_command);

  SCM_DEFER_INTS;
  c_client = (GnomeClient*)sgtk_get_gtkobj (p_client);
  set_prop_from_arglist (c_client, p_list, s_gnome_client_set_clone_command,
			 (set_prop_func)gnome_client_set_clone_command);
  SCM_ALLOW_INTS;

  return SCM_UNSPECIFIED;
}

SCM_PROC (s_gnome_client_set_environment, "gnome-client-set-environment", 2, 0, 0, sgnome_client_set_environment);
SCM
sgnome_client_set_environment (SCM p_client, SCM p_list)
{
  GnomeClient* c_client;
  SCM_ASSERT (sgtk_is_a_gtkobj (gnome_client_get_type (), p_client), p_client, SCM_ARG1, s_gnome_client_set_environment);

  SCM_DEFER_INTS;
  c_client = (GnomeClient*)sgtk_get_gtkobj (p_client);
  set_prop_from_arglist (c_client, p_list, s_gnome_client_set_environment,
			 gnome_client_set_environment);
  SCM_ALLOW_INTS;

  return SCM_UNSPECIFIED;
}

SCM_PROC (s_gnome_client_set_restart_command, "gnome-client-set-restart-command", 2, 0, 0, sgnome_client_set_restart_command);
SCM
sgnome_client_set_restart_command (SCM p_client, SCM p_list)
{
  GnomeClient* c_client;
  SCM_ASSERT (sgtk_is_a_gtkobj (gnome_client_get_type (), p_client), p_client, SCM_ARG1, s_gnome_client_set_restart_command);

  SCM_DEFER_INTS;
  c_client = (GnomeClient*)sgtk_get_gtkobj (p_client);
  set_prop_from_arglist (c_client, p_list, s_gnome_client_set_restart_command,
			 gnome_client_set_restart_command);
  SCM_ALLOW_INTS;

  return SCM_UNSPECIFIED;
}

SCM_PROC (s_gnome_client_set_resign_command, "gnome-client-set-resign-command", 2, 0, 0, sgnome_client_set_resign_command);
SCM
sgnome_client_set_resign_command (SCM p_client, SCM p_list)
{
  GnomeClient* c_client;
  SCM_ASSERT (sgtk_is_a_gtkobj (gnome_client_get_type (), p_client), p_client, SCM_ARG1, s_gnome_client_set_resign_command);

  SCM_DEFER_INTS;
  c_client = (GnomeClient*)sgtk_get_gtkobj (p_client);
  set_prop_from_arglist (c_client, p_list, s_gnome_client_set_resign_command,
			 gnome_client_set_resign_command);
  SCM_ALLOW_INTS;

  return SCM_UNSPECIFIED;
}

SCM_PROC (s_gnome_client_set_shutdown_command, "gnome-client-set-shutdown-command", 2, 0, 0, sgnome_client_set_shutdown_command);
SCM
sgnome_client_set_shutdown_command (SCM p_client, SCM p_list)
{
  GnomeClient* c_client;
  SCM_ASSERT (sgtk_is_a_gtkobj (gnome_client_get_type (), p_client), p_client, SCM_ARG1, s_gnome_client_set_shutdown_command);

  SCM_DEFER_INTS;
  c_client = (GnomeClient*)sgtk_get_gtkobj (p_client);
  set_prop_from_arglist (c_client, p_list, s_gnome_client_set_shutdown_command,
			 gnome_client_set_shutdown_command);
  SCM_ALLOW_INTS;

  return SCM_UNSPECIFIED;
}

void
gnome_guile_client_init ()
{
#ifndef SCM_MAGIC_SNARFER
#include "client.x"
#endif
}
