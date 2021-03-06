/* -*- Mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; c-basic-offset: 2; -*- */
#include <config.h>

/* needed for sigaction and friends under 'gcc -ansi -pedantic' on 
 * GNU/Linux */
#ifndef _POSIX_SOURCE
#  define _POSIX_SOURCE 1
#endif
#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>

#include <unistd.h>
#include <gnome.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  GtkWidget *mainwin, *urlbtn;
  gchar* msg;
  struct sigaction sa;
  poptContext ctx;
  const char **args;
  char *urlstr;
  const char *app_version = NULL;
  int res;
  gchar *appname;
  gchar *bug_buddy_path = NULL;
  gchar *debugger = NULL;
  gchar *debugger_path = NULL;
  
  int bb_sm_disable = 0;

  /* We do this twice to make sure we don't start running ourselves... :) */
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = SIG_IGN;
  sigaction(SIGSEGV, &sa, NULL);


  bindtextdomain (PACKAGE, GNOMELOCALEDIR);
  textdomain (PACKAGE);

  /* in case gnome-session is segfaulting :-) */
  gnome_client_disable_master_connection();

  putenv("GNOME_DISABLE_CRASH_DIALOG=1"); /* Don't recurse */  
  gnome_init_with_popt_table("gnome_segv", VERSION, argc, argv, NULL, 0, &ctx);

  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = SIG_IGN;
  sigaction(SIGSEGV, &sa, NULL);

  args = poptGetArgs(ctx);
  if (args && args[0] && args[1])
    {
      if (strcmp(g_filename_pointer (args[0]), "gnome-session") == 0)
        {
          msg = g_strdup_printf(_("The GNOME Session Manager (process %d) has crashed\n"
                                  "due to a fatal error (%s).\n"
                                  "When you close this dialog, all applications will close "
                                  "and your session will exit.\nPlease save all your files "
                                  "before closing this dialog."),
                                getppid(), g_strsignal(atoi(args[1])));
          bb_sm_disable = 1;
        }
      else
        {
          msg = g_strdup_printf(_("Application \"%s\" (process %d) has crashed\ndue to a fatal error.\n(%s)"),
                                args[0], getppid(), g_strsignal(atoi(args[1])));
        }
	if(args[2])
		app_version = args[2];
    }
  else
    {
      fprintf(stderr, _("Usage: gnome_segv appname signum\n"));
      return 1;
    }
  appname = g_strdup(args[0]);

  mainwin = gnome_message_box_new(msg,
                                  GNOME_MESSAGE_BOX_ERROR,
                                  GNOME_STOCK_BUTTON_CLOSE,
                                  NULL);
  
  bug_buddy_path = gnome_is_program_in_path ("bug-buddy");
  if (bug_buddy_path != NULL)
    {
      gnome_dialog_append_button(GNOME_DIALOG(mainwin),
                                 _("Submit a bug report"));
    }

  debugger = getenv("GNOME_DEBUGGER");
  if (debugger && strlen(debugger)>0)
  {
    debugger_path = gnome_is_program_in_path (debugger);
    if (debugger_path != NULL)
      {
        gnome_dialog_append_button(GNOME_DIALOG(mainwin),
                                   _("Debug"));
      }
  }
  
  /* Please download http://www.gnome.org/application_crashed-shtml.txt,
   * translate the plain text, and send the file to webmaster@gnome.org. */
  urlstr = g_strdup_printf("%s?app=%s%s%s&libsver=%s", 
		_("http://www.gnome.org/application_crashed.shtml"),
		args[0],
		app_version?"&version=":"",
		app_version?app_version:"",
		VERSION);
  urlbtn = gnome_href_new(urlstr,
                          _("Please visit the GNOME Application Crash page for more information"));
  gtk_widget_show(urlbtn);
  gtk_container_add(GTK_CONTAINER(GNOME_DIALOG(mainwin)->vbox), urlbtn);

  g_free(msg);

  res = gnome_dialog_run(GNOME_DIALOG(mainwin));

  if (res == 1 && (bug_buddy_path != NULL))
    {
      gchar *exec_str;
      int retval;

      g_assert(bug_buddy_path);
      exec_str = g_strdup_printf("%s --appname=\"%s\" --pid=%d "
                                 "--package-ver=\"%s\" %s", 
                                 bug_buddy_path, appname, getppid(), 
                                 app_version, bb_sm_disable 
                                 ? "--sm-disable" : "");

      retval = system(exec_str);
      g_free(exec_str);
      if (retval == -1 || retval == 127)
        {
          g_warning("Couldn't run bug-buddy: %s", g_strerror(errno));
        }
    }
  else if (res == 2 || (res == 1 && (bug_buddy_path == NULL)))
    {
      gchar *exec_str;
      int retval;

      g_assert (debugger_path);
      exec_str = g_strdup_printf("%s --appname=\"%s\" --pid=%d "
                                 "--package-ver=\"%s\" %s", 
                                 debugger_path, appname, getppid(), 
                                 app_version, bb_sm_disable 
                                 ? "--sm-disable" : "");

      retval = system(exec_str);
      g_free(exec_str);
      if (retval == -1 || retval == 127)
        {
          g_warning("Couldn't run debugger: %s", g_strerror(errno));
        }
    }

  return 0;
}
