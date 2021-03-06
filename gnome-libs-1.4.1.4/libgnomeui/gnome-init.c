/* -*- Mode: C; tab-width: 8; c-basic-offset: 8; indent-tabs-mode: nil -*- */
/* Blame Elliot for the poptimization of this file */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#define USE_SEGV_HANDLE 1

/* AIX requires this to be the first thing in the file.  */
#ifndef __GNUC__
# if HAVE_ALLOCA_H
#  include <alloca.h>
# else
#  ifdef _AIX
 #pragma alloca
#  else
#   ifndef alloca /* predefined by HP cc +Olibcalls */
char *alloca ();
#   endif
#  endif
# endif
#endif

#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <gtk/gtk.h>
#include <gdk_imlib.h>

#ifdef HAVE_ESD
#include <esd.h>
#endif

#include "libgnome/libgnomeP.h"
#include "libgnome/gnome-popt.h"
#include "gnome-preferences.h"
#include "libgnomeui/gnome-client.h"
#include "libgnomeui/gnome-init.h"
#include "libgnomeui/gnome-winhints.h"
#include "libgnomeui/gnome-window-icon.h"
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

static void initialize_gtk_signal_relay(void);
static gboolean
relay_gtk_signal(GtkObject *object,
		 guint signal_id,
		 guint n_params,
		 GtkArg *params,
		 gchar *signame);

extern char *program_invocation_name;
extern char *program_invocation_short_name;

extern void  gnome_type_init (void);

static void gnome_rc_parse(gchar *command);
static void gnome_segv_setup(gboolean post_arg_parse);

static GdkPixmap *imlib_image_loader(GdkWindow   *window,
				     GdkColormap *colormap,
				     GdkBitmap  **mask,
				     GdkColor    *transparent_color,
				     const gchar *filename);

/* This isn't conditionally compiled based on USE_SEGV_HANDLE
   because the --disable-crash-dialog option is always accepted,
   even if it has no effect
*/
gboolean disable_crash_dialog = FALSE;


/* The master client.  */
static GnomeClient *client = NULL;

static gboolean gnome_initialized = FALSE;

static void
gnome_add_gtk_arg_callback(poptContext con,
			   enum poptCallbackReason reason,
			   const struct poptOption * opt,
			   const char * arg, void * data)
{
        static int gnome_gtk_initialized = FALSE;
        static GPtrArray *gtk_args = NULL;
	char *newstr;
	int final_argc;
	char **final_argv;
	
	if (gnome_gtk_initialized) {
		/*
		 * gnome has already been initialized, so app might be making a
		 * second pass over the args - just ignore
		 */
		return;
	}
	
	switch(reason) {
	  gchar *file_name;
	case POPT_CALLBACK_REASON_PRE:
		gtk_args = g_ptr_array_new();
		
		/* Note that the value of argv[0] passed to main() may be
		 * different from the value that this passes to gtk
		 */
		g_ptr_array_add(gtk_args,
				(char *)poptGetInvocationName(con));
		break;
		
	case POPT_CALLBACK_REASON_OPTION:
	        switch(opt->argInfo) {
		case POPT_ARG_STRING:
		case POPT_ARG_INT:
		case POPT_ARG_LONG:
		  newstr = g_strconcat("--", opt->longName, "=", arg, NULL);
		  break;
		default:
		  newstr = g_strconcat("--", opt->longName, NULL);
		  break;
		}

		g_ptr_array_add(gtk_args, newstr);
		gnome_client_add_static_arg(client, newstr, NULL);
		break;
		
	case POPT_CALLBACK_REASON_POST:
		g_ptr_array_add(gtk_args, NULL);

		final_argc = gtk_args->len - 1;
		final_argv = g_memdup(gtk_args->pdata, sizeof(char *) * gtk_args->len);
		gtk_init(&final_argc, &final_argv);
		g_free(final_argv);
		
		g_ptr_array_free(gtk_args, TRUE);
		gtk_args = NULL;
		gnome_gtk_initialized = TRUE;
		file_name = g_concat_dir_and_file (gnome_user_accels_dir, gnome_app_id);
#if GTK_CHECK_VERSION(1,2,1)
		gtk_item_factory_parse_rc (file_name);
#else
		g_warning ("Accelerator support disabled.  Use at least GTK+ 1.2.1 if you want it.");
#endif
		g_free (file_name);
		break;
	}
}

static const struct poptOption gtk_options [] = {
        { NULL, '\0', POPT_ARG_INTL_DOMAIN, PACKAGE, 0, NULL, NULL},
	{ NULL, '\0', POPT_ARG_CALLBACK|POPT_CBFLAG_PRE|POPT_CBFLAG_POST,
	  &gnome_add_gtk_arg_callback, 0, NULL},
	{ "gdk-debug", '\0', POPT_ARG_STRING, NULL, 0,
	  N_("Gdk debugging flags to set"), N_("FLAGS")},
	{ "gdk-no-debug", '\0', POPT_ARG_STRING, NULL, 0,
	  N_("Gdk debugging flags to unset"), N_("FLAGS")},
	{ "display", '\0', POPT_ARG_STRING, NULL, 0,
	  N_("X display to use"), N_("DISPLAY")},
	{ "sync", '\0', POPT_ARG_NONE, NULL, 0,
	  N_("Make X calls synchronous"), NULL},
	{ "no-xshm", '\0', POPT_ARG_NONE, NULL, 0,
	  N_("Don't use X shared memory extension"), NULL},
	{ "name", '\0', POPT_ARG_STRING, NULL, 0,
	  N_("Program name as used by the window manager"), N_("NAME")},
	{ "class", '\0', POPT_ARG_STRING, NULL, 0,
	  N_("Program class as used by the window manager"), N_("CLASS")},
	{ "gxid_host", '\0', POPT_ARG_STRING, NULL, 0,
	  NULL, N_("HOST")},
	{ "gxid_port", '\0', POPT_ARG_STRING, NULL, 0,
	  NULL, N_("PORT")},
	{ "xim-preedit", '\0', POPT_ARG_STRING, NULL, 0,
	  NULL, N_("STYLE")},
	{ "xim-status", '\0', POPT_ARG_STRING, NULL, 0,
	  NULL, N_("STYLE")},
	{ "gtk-debug", '\0', POPT_ARG_STRING, NULL, 0,
	  N_("Gtk+ debugging flags to set"), N_("FLAGS")},
	{ "gtk-no-debug", '\0', POPT_ARG_STRING, NULL, 0,
	  N_("Gtk+ debugging flags to unset"), N_("FLAGS")},
	{ "g-fatal-warnings", '\0', POPT_ARG_NONE, NULL, 0,
	  N_("Make all warnings fatal"), NULL},
	{ "gtk-module", '\0', POPT_ARG_STRING, NULL, 0,
	  N_("Load an additional Gtk module"), N_("MODULE")},
	{ NULL, '\0', 0, NULL, 0}
};

enum {
        KEY_DISABLE_CRASH_DIALOG = -2,
        KEY_GEOMETRY = -3
};

static void
gnome_init_cb(poptContext ctx, enum poptCallbackReason reason,
	      const struct poptOption *opt)
{
	if (gnome_initialized)
		return;
	
	switch(reason) {
	case POPT_CALLBACK_REASON_PRE:
                {
                        char *ctype, *old_ctype = NULL;
                        gboolean ctype_set;

                        gnome_segv_setup (FALSE);
                        ctype = setlocale (LC_CTYPE, NULL);

                        if (!strcmp(ctype, "C")) {
                                old_ctype = g_strdup (getenv ("LC_CTYPE"));
                                putenv ("LC_CTYPE=en_US");
                                ctype_set = TRUE;
                        } else
                                ctype_set = FALSE;

                        gtk_set_locale ();

                        if (ctype_set) {
                                char *setme;

                                if (old_ctype) {
                                        setme = g_strconcat ("LC_CTYPE=", old_ctype, NULL);
                                        g_free(old_ctype);
                                } else
                                        setme = "LC_CTYPE=";

                                putenv (setme);
                        }
                        client = gnome_master_client();
                }
		break;
	case POPT_CALLBACK_REASON_POST:
		gdk_imlib_init();
		gnome_type_init();
		gtk_rc_set_image_loader(imlib_image_loader);
		gnome_rc_parse(program_invocation_name);
		gnome_preferences_load();
		/*
		 * No, this is not the most evil thing you have
		 * seen raster.  Imlib is the most evil thing
	   	 * we have seen.  And until the cache in imlib
	 	 * is smarter, this stays.
		 *
		 * Last I checked, there was no imlib-capplet
		 */
		if (gnome_preferences_get_disable_imlib_cache ()){
			int pixmaps, images;
			
			/*
			 * If cache info has been set to -1, -1, it
			 * means something initialized before us and
			 * is requesting the cache to not be touched
			 */
			gdk_imlib_get_cache_info (&pixmaps, &images);

			if (!(pixmaps == -1 || images == -1))
				gdk_imlib_set_cache_info (0, images);
		}

                gnome_segv_setup(TRUE);
		
		/* lame trigger stuff. This really needs to be sorted
		   out better so that the trigger API is exported more
		   - perhaps there's something hidden in glib that
		   already does this :) */

		initialize_gtk_signal_relay();
		
		gnome_initialized = TRUE;
		break;

	case POPT_CALLBACK_REASON_OPTION:
                if (opt->val == KEY_DISABLE_CRASH_DIALOG) {
                        disable_crash_dialog = TRUE;
                }
	default:
		break;
	}
}

static const struct poptOption gnome_options[] = {
        { NULL, '\0', POPT_ARG_INTL_DOMAIN, PACKAGE, 0, NULL, NULL},
	{NULL, '\0', POPT_ARG_CALLBACK|POPT_CBFLAG_PRE|POPT_CBFLAG_POST,
	 &gnome_init_cb, 0, NULL, NULL},
	{"disable-crash-dialog", '\0', POPT_ARG_NONE, NULL, KEY_DISABLE_CRASH_DIALOG},
	{NULL, '\0', 0, NULL, 0}
};

static void
gnome_register_options(void)
{
	gnomelib_register_popt_table(gtk_options, N_("GTK options"));
	gnomelib_register_popt_table(gnome_options, N_("GNOME GUI options"));
}

/* #define ALLOC_DEBUGGING_HOOKS */
#ifdef ALLOC_DEBUGGING_HOOKS
#include <malloc.h>
#include <sys/time.h>

static struct mallinfo mi1, mi2;

#define AM() mi1 = mallinfo();
#define PM(x) mi2 = mallinfo(); printf(x ": used %d, now %d\n", \
mi2.uordblks - mi1.uordblks, mi2.uordblks);

typedef struct {
  gulong magic;
  gulong bsize;
  struct timeval tv;
} BlkInfo;

static void (*old_free_hook)(__malloc_ptr_t __ptr);
static __malloc_ptr_t (*old_malloc_hook) (size_t __size);
static __malloc_ptr_t (*old_realloc_hook) (__malloc_ptr_t __ptr,
					   size_t __size);

static void my_free_hook(__malloc_ptr_t ptr)
{
  char buf[512];
  BlkInfo *tv;
  struct timeval now;

  tv = (BlkInfo *)(((char *)ptr) - sizeof(BlkInfo));

  __free_hook = old_free_hook;
  if(tv->magic != 0xadaedead) {
    goto out;
  }

  gettimeofday(&now, NULL);
  sprintf(buf, "-%p %ld %ld\n", ptr, tv->bsize,
	  (now.tv_sec*1000000 + now.tv_usec)
	  - (tv->tv.tv_sec*1000000 + tv->tv.tv_usec));
  write(1, buf, strlen(buf));

  free(tv);
 out:
  __free_hook = my_free_hook;
}

static __malloc_ptr_t my_malloc_hook(size_t bsize)
{
  BlkInfo *newblk;
  __malloc_ptr_t retval;
  char buf[512];

  if(!bsize) return NULL;

  __malloc_hook = old_malloc_hook;

  newblk = malloc(bsize+sizeof(BlkInfo));

  if(newblk) {
    newblk->magic = 0xadaedead;
    newblk->bsize = bsize;
    gettimeofday(&newblk->tv, NULL);
  }


  retval = newblk;
  retval += sizeof(BlkInfo);

  sprintf(buf, "+%p %ld\n", retval, bsize);
  write(1, buf, strlen(buf));

  __malloc_hook = my_malloc_hook;
  return retval;
}

static __malloc_ptr_t my_realloc_hook(__malloc_ptr_t __ptr,
				      size_t __size)
{
  BlkInfo *oldblk, *newblk;
  __malloc_ptr_t rv;

  __realloc_hook = old_realloc_hook;
  oldblk = (((char *)__ptr) - sizeof(BlkInfo));

  if(__ptr && oldblk->magic == 0xadaedead) {
    newblk = realloc(oldblk, __size + sizeof(BlkInfo));
    rv = newblk + 1;
    newblk->bsize = __size;
  } else
    rv = realloc(__ptr, __size);

  __realloc_hook = my_realloc_hook;

  return rv;
}
#endif

/**
 * gnome_init_with_popt_table:
 * @app_id: Application id.
 * @app_version: Application version.
 * @argc: argument count (for example argc as received by main)
 * @argv: argument vector (for example argv as received by main)
 * @options: poptOption table with options to parse
 * @flags: popt flags.
 * @return_ctx: if non-NULL, the popt context is returned here.
 *
 * Initializes the application.  This sets up all of the GNOME
 * internals and prepares them (imlib, gdk, session-management, triggers,
 * sound, user preferences)
 *
 * Unlike gnome_init, with gnome_init_with_popt_table you can provide
 * a table of popt options (popt is the command line argument parsing
 * library).
 */

int
gnome_init_with_popt_table(const char *app_id,
			   const char *app_version,
			   int argc, char **argv,
			   const struct poptOption *options,
			   int flags,
			   poptContext *return_ctx)
{
	poptContext ctx;
	char *appdesc = NULL;

#ifdef ALLOC_DEBUGGING_HOOKS
	appdesc = malloc(10);
	appdesc = realloc(appdesc, 20);
	free(appdesc);
	
	old_free_hook = __free_hook;
	old_malloc_hook = __malloc_hook;
	old_realloc_hook = __realloc_hook;
	__free_hook = my_free_hook;
	__malloc_hook = my_malloc_hook;
	__realloc_hook = my_realloc_hook;
#endif
	if (gnome_initialized)
                return 0;
	
	gnomelib_init (app_id, app_version);
	
	gnome_register_options();

	if (getenv ("GTK_DEBUG_OBJECTS"))
		gtk_debug_flags |= GTK_DEBUG_OBJECTS;
		
	gnome_client_init();
	
	if(options) {
		appdesc = g_strdup_printf(_("%s options"), app_id);
		gnomelib_register_popt_table(options, appdesc);
	}
	
	ctx = gnomelib_parse_args(argc, argv, flags);

        /* free this after we parse the args to aviod a crash */
        g_free (appdesc);

	if(return_ctx)
		*return_ctx = ctx;
	else
		poptFreeContext(ctx);

        gnome_window_icon_init ();

	/* reduce mem usage (hopefully) */
	gnome_config_sync();
	g_blow_chunks();

	return 0;
}

/**
 * gnome_init:
 * @app_id: Application id.
 * @app_version: Application version.
 * @argc: argument count (for example argc as received by main)
 * @argv: argument vector (for example argv as received by main)
 *
 * Initializes the application.  This sets up all of the GNOME
 * internals and prepares them (imlib, gdk, session-management, triggers,
 * sound, user preferences)
 */
int
gnome_init(const char *app_id,
	   const char *app_version,
	   int argc, char **argv)
{
  if (gnome_initialized)
          return 0;

  gnome_init_with_popt_table(app_id, app_version,
			     argc, argv, NULL, 0, NULL);

  return 0;
}

/* perhaps this belongs in libgnome.. move it if you like. */

/* automagically parse all the gtkrc files for us.
 * 
 * Parse:
 * $gnomedatadir/gtkrc
 * $gnomedatadir/$apprc
 * ~/.gnome/gtkrc
 * ~/.gnome/$apprc
 *
 * appname is derived from argv[0].  IMHO this is a great solution.
 * It provides good consistancy (you always know the rc file will be
 * the same name as the executable), and it's easy for the programmer.
 * 
 * If you don't like it.. give me a good reason.  Symlin
 */
static void
gnome_rc_parse (gchar *command)
{
	gint i;
	gint buf_len;
	gint found = 0;
	gchar *buf = NULL;
	gchar *file;
	gchar *apprc;
	
	buf_len = strlen(command);
	
	for (i = 0; i < buf_len; i++) {
		if (command[buf_len - i] == '/') {
			buf = g_strdup (&command[buf_len - i + 1]);
			found = TRUE;
			break;
		}
	}
	
	if (!found)
		buf = g_strdup (command);
	
	apprc = g_malloc (strlen(buf) + 3);
	g_snprintf(apprc, strlen(buf) + 3, "%src", buf);
	
	g_free(buf);
	
	
	/* <gnomedatadir>/gtkrc */
	file = gnome_unconditional_datadir_file("gtkrc");
	if (file){
		gtk_rc_add_default_file (file);
		g_free (file);
	}

	/* <gnomedatadir>/<progname> */
	file = gnome_unconditional_datadir_file(apprc);
	if (file){
		gtk_rc_add_default_file (file);
		g_free (file);
	}
	
	/* ~/.gnome/gtkrc */
	file = gnome_util_home_file("gtkrc");
	if (file){
		gtk_rc_add_default_file (file);
		g_free (file);
	}
	
	/* ~/.gnome/<progname> */
	file = gnome_util_home_file(apprc);
	if (file){
		gtk_rc_add_default_file (file);
		g_free (file);
	}
	
	g_free (apprc);
	gtk_rc_init ();
}


#ifdef USE_SEGV_HANDLE
static void gnome_segv_handle(int signum);
#endif

static void
gnome_segv_setup(gboolean post_arg_parse)
{
#ifdef USE_SEGV_HANDLE
        static struct sigaction sa_saved_fpe, sa_saved_segv, sa_saved_bus;
        struct sigaction sa;

        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = (gpointer)gnome_segv_handle;
 
        /* 
         * Yes, we do this twice, so if an error occurs before init,
         * it will be caught, and if it happens after init, we'll override
         * gtk's handler
         */
       if(post_arg_parse) {
                struct sigaction *setptr_segv, *setptr_fpe, *setptr_bus;

                if(disable_crash_dialog) {
                        setptr_segv = &sa_saved_segv;
                        setptr_fpe = &sa_saved_fpe;
                        setptr_bus = &sa_saved_bus;
                } else
                        setptr_segv = setptr_fpe = setptr_bus = &sa;

                sigaction(SIGSEGV, setptr_segv, NULL);
                sigaction(SIGFPE, setptr_fpe, NULL);
                sigaction(SIGBUS, setptr_bus, NULL);
        } else {
                struct sigaction *setptr;
                char *ctmp;

                /* Well, actually we haven't parsed options yet
                   so --disable-crash-dialog can't take effect.
                   But you could change this flag in the debugger. -hp

                   ...or use an environment variable - ecl
                */
                ctmp = getenv("GNOME_DISABLE_CRASH_DIALOG");
                if(ctmp)
                        disable_crash_dialog = atoi(ctmp)?TRUE:FALSE;

                setptr = disable_crash_dialog?NULL:&sa;

                sigaction(SIGSEGV, setptr, &sa_saved_segv);
                sigaction(SIGFPE, setptr, &sa_saved_fpe);
                sigaction(SIGBUS, setptr, &sa_saved_bus);
        }
#endif
}

#ifdef USE_SEGV_HANDLE
static void gnome_segv_handle(int signum)
{
	static int in_segv = 0;
	pid_t pid;
	
	in_segv++;

        if (in_segv > 2) {
                /* The fprintf() was segfaulting, we are just totally hosed */
                _exit(1);
        } else if (in_segv > 1) {
                /* dialog display isn't working out */
                fprintf(stderr, _("Multiple segmentation faults occurred; can't display error dialog\n"));
                _exit(1);
        }

        /* Make sure we release grabs */
        gdk_pointer_ungrab(GDK_CURRENT_TIME);
        gdk_keyboard_ungrab(GDK_CURRENT_TIME);

        gdk_flush();
        
	if ((pid = fork())) {
                /* Wait for user to see the dialog, then exit. */
                /* Why wait at all? Because we want to allow people to attach to the
		   process */
		int estatus;
		pid_t eret;

		eret = waitpid(pid, &estatus, 0);

		if(getenv("GNOME_DUMP_CORE"))		
	                abort ();
		_exit(1);
	} else {
		char buf[32];

		g_snprintf(buf, sizeof(buf), "%d", signum);

		/* Child process */
		execl(GNOMEBINDIR "/gnome_segv", GNOMEBINDIR "/gnome_segv",
		      program_invocation_name, buf, gnome_app_version, NULL);

                execlp("gnome_segv", "gnome_segv", program_invocation_name, buf, gnome_app_version, NULL);

                _exit(99);
	}

	in_segv--;
}
#endif

static GdkPixmap *
imlib_image_loader(GdkWindow   *window,
		   GdkColormap *colormap,
		   GdkBitmap  **mask,
		   GdkColor    *transparent_color,
		   const gchar *filename)
{
	GdkPixmap *retval;
	
	if (gdk_imlib_load_file_to_pixmap ((char *) filename, &retval, mask)) {
                if (retval) {
                        if (mask && *mask)
                                gdk_pixmap_ref (*mask);
                        gdk_pixmap_ref (retval);
                        gdk_imlib_free_pixmap (retval);
                } else {
                        /* Paranoia */
                        if (mask && *mask) {
                                gdk_imlib_free_pixmap (*mask);
                                *mask = NULL;
                        }
                }
		return retval;
        }
	else
		return NULL;
}

static gboolean
relay_gtk_signal(GtkObject *object,
		 guint signal_id,
		 guint n_params,
		 GtkArg *params,
		 gchar *signame)
{
#ifdef HAVE_ESD
  char *pieces[3] = {"gtk-events", NULL, NULL};
  static GQuark disable_sound_quark = 0;

  pieces[1] = signame;

  if(!disable_sound_quark)
    disable_sound_quark = g_quark_from_static_string("gnome_disable_sound_events");

  if(gtk_object_get_data_by_id(object, disable_sound_quark))
    return TRUE;

  if(GTK_IS_WIDGET(object)) {
    if(!GTK_WIDGET_DRAWABLE(object))
      return TRUE;

    if(GTK_IS_MENU_ITEM(object) && GTK_MENU_ITEM(object)->submenu)
      return TRUE;
  }

  gnome_triggers_vdo("", NULL, (const char **)pieces);

  return TRUE;
#else
  return FALSE; /* this shouldn't even happen... */
#endif
}

static void
initialize_gtk_signal_relay (void)
{
#ifdef HAVE_ESD
	gpointer iter_signames;
	char *signame;
	char *ctmp, *ctmp2;
	
	if (gnome_sound_connection < 0)
		return;
	
	if (!gnome_config_get_bool ("/sound/system/settings/event_sounds=true"))
		return;
	
	ctmp = gnome_config_file ("/sound/events/gtk-events.soundlist");
	ctmp2 = g_strconcat ("=", ctmp, "=", NULL);
	g_free (ctmp);
	iter_signames = gnome_config_init_iterator_sections (ctmp2);
	gnome_config_push_prefix (ctmp2);
	g_free (ctmp2);
	
	while ((iter_signames = gnome_config_iterator_next (iter_signames,
							    &signame, NULL))) {
		int signums [5];
		int nsigs, i;
		gboolean used_signame;

		/*
		 * XXX this is an incredible hack based on a compile-time
		 * knowledge of what gtk widgets do what, rather than
		 * anything based on the info available at runtime.
		 */
		if(!strcmp (signame, "activate")){
			gtk_type_class (gtk_menu_item_get_type ());
			signums [0] = gtk_signal_lookup (signame, gtk_menu_item_get_type ());
			
			gtk_type_class (gtk_editable_get_type ());
			signums [1] = gtk_signal_lookup (signame, gtk_editable_get_type ());
			nsigs = 2;
		} else if(!strcmp(signame, "toggled")){
			gtk_type_class (gtk_toggle_button_get_type ());
			signums [0] = gtk_signal_lookup (signame,
							 gtk_toggle_button_get_type ());
			
			gtk_type_class (gtk_check_menu_item_get_type ());
			signums [1] = gtk_signal_lookup (signame,
							 gtk_check_menu_item_get_type ());
			nsigs = 2;
		} else if (!strcmp (signame, "clicked")){
			gtk_type_class (gtk_button_get_type ());
			signums [0] = gtk_signal_lookup (signame, gtk_button_get_type ());
			nsigs = 1;
		} else {
			gtk_type_class (gtk_widget_get_type ());
			signums [0] = gtk_signal_lookup (signame, gtk_widget_get_type ());
			nsigs = 1;
		}

		used_signame = FALSE;
		for(i = 0; i < nsigs; i++) {
			if (signums [i] > 0) {
				gtk_signal_add_emission_hook (signums [i],
							      (GtkEmissionHook)relay_gtk_signal,
							      signame);
                                used_signame = TRUE;
                        }
                }

                if(!used_signame)
                        g_free(signame);
	}
	gnome_config_pop_prefix ();
	
#endif
}
