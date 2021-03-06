/*   Display & Edit Journal of Timestamps for GTimeTracker - a time tracker
 *   Copyright (C) 2001 Linas Vepstas
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define _GNU_SOURCE

#include "config.h"

#include <errno.h>
#include <glade/glade.h>
#include <gnome.h>
#include <gtkhtml/gtkhtml.h>
#include <stdio.h>
#include <string.h>

#include "app.h"
#include "ctree.h"
#include "journal.h"
#include "ghtml.h"
#include "proj.h"
#include "props-invl.h"
#include "props-task.h"
#include "util.h"


/* this struct is a random mish-mash of stuff, not well organized */

typedef struct wiggy_s {
	GttGhtml *gh;    
	GtkHTML *htmlw;
	GtkHTMLStream *handle;
	GtkWidget *top;
	GtkWidget *interval_popup;
	GtkWidget *interval_paste;
	GtkWidget *interval_merge_up;
	GtkWidget *interval_merge_down;
	GtkWidget *task_popup;
	GtkWidget *task_delete_memo;
	GtkWidget *task_paste;
	GtkFileSelection *filesel;
	EditIntervalDialog *edit_ivl;
	char * filepath;
	GttInterval * interval;
	GttTask * task;
	GttProject *prj;
} Wiggy;

/* ============================================================== */
/* html i/o routines */

static void
wiggy_open (GttGhtml *pl, gpointer ud)
{
	Wiggy *wig = (Wiggy *) ud;

	/* open the browser for writing */
	wig->handle = gtk_html_begin_content(wig->htmlw, "text/html");
}

static void
wiggy_close (GttGhtml *pl, gpointer ud)
{
	Wiggy *wig = (Wiggy *) ud;

	/* close the browser stream */
	gtk_html_end (wig->htmlw, wig->handle, GTK_HTML_STREAM_OK);
	wig->handle = NULL;
}

static void
wiggy_write (GttGhtml *pl, const char *str, size_t len, gpointer ud)
{
	Wiggy *wig = (Wiggy *) ud;

	/* write to the browser stream */
	gtk_html_write (wig->htmlw, wig->handle, str, len);
}

static void
wiggy_error (GttGhtml *pl, int err, const char * msg, gpointer ud)
{
	Wiggy *wig = (Wiggy *) ud;
	GtkHTML *htmlw = wig->htmlw;
	GtkHTMLStream *han;
	char buff[1000], *p;

	han = gtk_html_begin_content(htmlw, "text/html");

	if (404 == err)
	{
		p = buff;
		p = stpcpy (p, "<html><body><h1>");
		p = stpcpy (p, _("Error 404 Not Found"));
		p = stpcpy (p, "</h1>");
		p += sprintf (p, _("The file %s was not found."),
		             (msg? (char*) msg : _("(null)")));
		
		p = stpcpy (p, "</body></html>");
		gtk_html_write (htmlw, han, buff, p-buff);
	}
	else
	{
		p = buff;
		p = stpcpy (p, "<html><body><h1>");
		p = stpcpy (p, _("Unkown Error"));
		p = stpcpy (p, "</h1></body></html>");
		gtk_html_write (htmlw, han, buff, p-buff);
	}
	
	gtk_html_end (htmlw, han, GTK_HTML_STREAM_OK);

}

/* ============================================================== */
/* engine callbacks */

static void 
redraw (GttProject * prj, gpointer data)
{
	Wiggy *wig = (Wiggy *) data;

	gtt_ghtml_display (wig->gh, wig->filepath, wig->prj);
}

/* ============================================================== */
/* file selection callbacks */

static gboolean
raw_html_receiver (gpointer     engine,
                   const gchar *data,
                   size_t       len,
                   gpointer     user_data) 
{
	FILE *fh = (FILE *) user_data;
	fwrite (data, len, 1, fh);
	return TRUE;
}

static void 
filesel_ok_clicked_cb (GtkWidget *w, gpointer data)
{
	Wiggy *wig = (Wiggy *) data;
	FILE *fh;
	char * filename;

	filename = gtk_file_selection_get_filename (wig->filesel);

	fh = fopen (filename, "w");
	if (!fh) 
	{
		gchar *msg;
		GtkWidget *mb;
		int nerr = errno;
		msg = g_strdup_printf (_("Unable to open the file %s\n%s"),
			filename, strerror (nerr)); 
		mb = gnome_message_box_new (msg,
			GNOME_MESSAGE_BOX_ERROR, 
			GNOME_STOCK_BUTTON_CLOSE,
			NULL);
		gtk_widget_show (mb);
		// g_free (msg);
	}
	else
	{
		gtk_html_save (wig->htmlw, raw_html_receiver, fh);
		fclose (fh);
	}

	gtk_widget_destroy (GTK_WIDGET(wig->filesel));
	wig->filesel = NULL;
}

static void 
filesel_cancel_clicked_cb (GtkWidget *w, gpointer data)
{
	Wiggy *wig = (Wiggy *) data;
	gtk_widget_destroy (GTK_WIDGET(wig->filesel));
	wig->filesel = NULL;
}

/* ============================================================== */
/* global clipboard, allows cut task to be reparented to a different project */

static GttTask * cutted_task = NULL;

/* ============================================================== */
/* interval popup actions */

static void
interval_new_clicked_cb (GtkWidget * w, gpointer data)
{
	Wiggy *wig = (Wiggy *) data;

	if (NULL == wig->edit_ivl) wig->edit_ivl = edit_interval_dialog_new();

	wig->interval = gtt_interval_new_insert_after(wig->interval);
	edit_interval_set_interval (wig->edit_ivl, wig->interval);
	edit_interval_dialog_show (wig->edit_ivl);
}

static void
interval_edit_clicked_cb(GtkWidget * dw, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;

	if (NULL == wig->edit_ivl) wig->edit_ivl = edit_interval_dialog_new();
	edit_interval_set_interval (wig->edit_ivl, wig->interval);
	edit_interval_dialog_show (wig->edit_ivl);
}

static void
interval_delete_clicked_cb(GtkWidget * w, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;
	gtt_interval_destroy (wig->interval);
	wig->interval = NULL;
}

static void
interval_merge_up_clicked_cb(GtkWidget * w, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;
	gtt_interval_merge_up (wig->interval);
	wig->interval = NULL;
}

static void
interval_merge_down_clicked_cb(GtkWidget * w, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;
	gtt_interval_merge_down (wig->interval);
	wig->interval = NULL;
}

static void
interval_insert_memo_cb(GtkWidget * w, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;
	GttTask *newtask;
	if (!wig->interval) return;

	/* try to get billrates consistent across gap */
	newtask = gtt_interval_get_parent (wig->interval);
	newtask = gtt_task_dup (newtask);
	gtt_task_set_memo (newtask, _("New Diary Entry"));
	gtt_task_set_notes (newtask, "");

	gtt_interval_split (wig->interval, newtask);
	prop_task_dialog_show (newtask);
}

static void
interval_paste_memo_cb(GtkWidget * w, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;
	GttTask *newtask;

	if (!cutted_task || !wig->interval) return;
	newtask = cutted_task;
	cutted_task = gtt_task_dup (newtask);
	
	gtt_interval_split (wig->interval, newtask);
}

static void
interval_popup_cb (Wiggy *wig)
{
	gtk_menu_popup(GTK_MENU(wig->interval_popup), 
		NULL, NULL, NULL, wig, 1, 0);
	if (cutted_task)
	{
		gtk_widget_set_sensitive (wig->interval_paste, TRUE);
	}
	else 
	{
		gtk_widget_set_sensitive (wig->interval_paste, FALSE);
	}

	if (gtt_interval_is_first_interval (wig->interval))
	{
		gtk_widget_set_sensitive (wig->interval_merge_up, FALSE);
	}
	else
	{
		gtk_widget_set_sensitive (wig->interval_merge_up, TRUE);
	}

	if (gtt_interval_is_last_interval (wig->interval))
	{
		gtk_widget_set_sensitive (wig->interval_merge_down, FALSE);
	}
	else
	{
		gtk_widget_set_sensitive (wig->interval_merge_down, TRUE);
	}
}

/* ============================================================== */

void
change_task(GtkWidget *w, gpointer data)
{
	GttProject *prj;
	GttTask *newtask;

	prj = ctree_get_focus_project (global_ptw);
	if (!prj) return;

	newtask = gtt_task_new ();
	gtt_project_prepend_task (prj, newtask);
	prop_task_dialog_show (newtask);
}

/* ============================================================== */

static void
task_new_task_clicked_cb(GtkWidget * w, gpointer data) 
{
	GttTask *newtask;
	Wiggy *wig = (Wiggy *) data;
	newtask = gtt_task_new_insert (wig->task);
	prop_task_dialog_show (newtask);
}

static void
task_edit_task_clicked_cb(GtkWidget * w, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;
	prop_task_dialog_show (wig->task);
}

static void
task_delete_memo_clicked_cb(GtkWidget * w, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;

	/* its physically impossible to cut just the memo,
	 * when its the first one */
	if (gtt_task_is_first_task (wig->task)) return;

	gtt_task_merge_up (wig->task);

	if (cutted_task)
	{
		gtt_task_destroy (cutted_task);
	}
	cutted_task = wig->task;
	gtt_task_remove (cutted_task);
}

static void
task_delete_times_clicked_cb(GtkWidget * w, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;
	if (cutted_task)
	{
		gtt_task_destroy (cutted_task);
	}
	cutted_task = wig->task;
	gtt_task_remove (cutted_task);
}

static void
task_paste_clicked_cb(GtkWidget * w, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;
	GttTask *task;

	if (!cutted_task || !wig->task) return;
	task = cutted_task;
	cutted_task = gtt_task_dup (task);
	
	gtt_task_insert (wig->task, task);
}

static void
task_new_interval_cb (GtkWidget * w, gpointer data)
{
	Wiggy *wig = (Wiggy *) data;

	if (NULL == wig->edit_ivl) wig->edit_ivl = edit_interval_dialog_new();

	wig->interval = gtt_interval_new();
	gtt_task_add_interval (wig->task, wig->interval);

	edit_interval_set_interval (wig->edit_ivl, wig->interval);
	edit_interval_dialog_show (wig->edit_ivl);
}

static void
task_popup_cb (Wiggy *wig)
{
	gtk_menu_popup(GTK_MENU(wig->task_popup), 
		NULL, NULL, NULL, wig, 1, 0);
	if (gtt_task_is_first_task (wig->task))
	{
		gtk_widget_set_sensitive (wig->task_delete_memo, FALSE);
	}
	else 
	{
		gtk_widget_set_sensitive (wig->task_delete_memo, TRUE);
	}

	if (cutted_task)
	{
		gtk_widget_set_sensitive (wig->task_paste, TRUE);
	}
	else 
	{
		gtk_widget_set_sensitive (wig->task_paste, FALSE);
	}

}

/* ============================================================== */

static void 
on_print_clicked_cb (GtkWidget *w, gpointer data)
{
	GladeXML  *glxml;
	glxml = gtt_glade_xml_new ("glade/not-implemented.glade", "Not Implemented");
}

static void 
on_save_clicked_cb (GtkWidget *w, gpointer data)
{
	GtkWidget *fselw;
	Wiggy *wig = (Wiggy *) data;

	/* don't show dialog more than once */
	if (wig->filesel) return;

	fselw = gtk_file_selection_new (_("Save HTML To File"));
	wig->filesel = GTK_FILE_SELECTION(fselw);

	gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(fselw)->ok_button), 
		"clicked", GTK_SIGNAL_FUNC(filesel_ok_clicked_cb), wig);

	gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(fselw)->cancel_button), 
		"clicked", GTK_SIGNAL_FUNC(filesel_cancel_clicked_cb), wig);

	gtk_widget_show (fselw);
}

static void 
on_close_clicked_cb (GtkWidget *w, gpointer data)
{
	Wiggy *wig = (Wiggy *) data;

	/* close the main journal window ... everything */
	gtt_project_remove_notifier (wig->prj, redraw, wig);
	edit_interval_dialog_destroy (wig->edit_ivl);
	gtk_widget_destroy (wig->top);
	gtt_ghtml_destroy (wig->gh);
	g_free (wig->filepath);
	g_free (wig);
}

static void 
on_refresh_clicked_cb (GtkWidget *w, gpointer data)
{
	Wiggy *wig = (Wiggy *) data;
	redraw (wig->prj, data);
}

/* ============================================================== */
/* html events */

static void
html_link_clicked_cb(GtkHTML * html, const gchar * url, gpointer data) 
{
	Wiggy *wig = (Wiggy *) data;
	gpointer addr = NULL;
	char *str;

	/* decode the address buried in the URL (if its there) */
	str = strstr (url, "0x");
	if (str)
	{
		addr = (gpointer) strtoul (str, NULL, 16);
	}

	if (0 == strncmp (url, "gtt:interval", 12))
	{
		wig->interval = addr;
		wig->task = NULL;
		if (addr) interval_popup_cb (wig);
	}
	else
	if (0 == strncmp (url, "gtt:task", 8))
	{
		wig->task = addr;
		wig->interval = NULL;
		if (addr) task_popup_cb (wig);
	}
	else
	{
		g_warning ("clicked on unknown link %s\n", url);
	}
}

static void
html_on_url_cb(GtkHTML * html, const gchar * url, gpointer data) 
{
	// printf ("hover over the url show hover help duude=%s\n", url);
}


/* ============================================================== */

static void
do_show_report (const char * report, GttProject *prj)
{
	GtkWidget *jnl_top, *jnl_viewport, *jnl_browser;
	GladeXML  *glxml;
	Wiggy *wig;

	glxml = gtt_glade_xml_new ("glade/journal.glade", "Journal Window");

	/* FIXME: disable print button for now: */
	{
		GtkWidget *w = glade_xml_get_widget (glxml, "print");
		if (w != NULL)
			gtk_widget_hide (w);
	}
	  
	jnl_top = glade_xml_get_widget (glxml, "Journal Window");
	jnl_viewport = glade_xml_get_widget (glxml, "Journal ScrollWin");

	/* create browser, plug it into the viewport */
	jnl_browser = gtk_html_new();
	gtk_container_add(GTK_CONTAINER(jnl_viewport), jnl_browser);

	/* ---------------------------------------------------- */
	/* signals for the browser, and the journal window */

	wig = g_new0 (Wiggy, 1);
	wig->edit_ivl = NULL;
	wig->filesel = NULL;

	wig->top = jnl_top;
	wig->htmlw = GTK_HTML(jnl_browser);

	wig->gh = gtt_ghtml_new();
	gtt_ghtml_set_stream (wig->gh, wig, wiggy_open, wiggy_write, 
		wiggy_close, wiggy_error);
	
	glade_xml_signal_connect_data (glxml, "on_close_clicked",
	        GTK_SIGNAL_FUNC (on_close_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_save_clicked",
	        GTK_SIGNAL_FUNC (on_save_clicked_cb), wig);

	glade_xml_signal_connect_data (glxml, "on_print_clicked",
	        GTK_SIGNAL_FUNC (on_print_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_refresh_clicked",
	        GTK_SIGNAL_FUNC (on_refresh_clicked_cb), wig);
	  
	gtk_signal_connect(GTK_OBJECT(jnl_browser), "link_clicked",
		GTK_SIGNAL_FUNC(html_link_clicked_cb), wig);

	gtk_signal_connect(GTK_OBJECT(jnl_browser), "on_url",
		GTK_SIGNAL_FUNC(html_on_url_cb), wig);


	gtk_widget_show (jnl_browser);
	gtk_widget_show (jnl_top);

	/* ---------------------------------------------------- */
	/* this is the popup menu that says 'edit/delete/merge' */
	/* for intervals */

	glxml = gtt_glade_xml_new ("glade/interval_popup.glade", "Interval Popup");
	wig->interval_popup = glade_xml_get_widget (glxml, "Interval Popup");
	wig->interval_paste = glade_xml_get_widget (glxml, "paste_memo");
	wig->interval_merge_up = glade_xml_get_widget (glxml, "merge_up");
	wig->interval_merge_down = glade_xml_get_widget (glxml, "merge_down");
	wig->interval=NULL;

	glade_xml_signal_connect_data (glxml, "on_new_interval_activate",
	        GTK_SIGNAL_FUNC (interval_new_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_edit_activate",
	        GTK_SIGNAL_FUNC (interval_edit_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_delete_activate",
	        GTK_SIGNAL_FUNC (interval_delete_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_merge_up_activate",
	        GTK_SIGNAL_FUNC (interval_merge_up_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_merge_down_activate",
	        GTK_SIGNAL_FUNC (interval_merge_down_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_insert_memo_activate",
	        GTK_SIGNAL_FUNC (interval_insert_memo_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_paste_memo_activate",
	        GTK_SIGNAL_FUNC (interval_paste_memo_cb), wig);
	  
	/* ---------------------------------------------------- */
	/* this is the popup menu that says 'edit/delete/merge' */
	/* for tasks */

	glxml = gtt_glade_xml_new ("glade/task_popup.glade", "Task Popup");
	wig->task_popup = glade_xml_get_widget (glxml, "Task Popup");
	wig->task_delete_memo = glade_xml_get_widget (glxml, "delete_memo");
	wig->task_paste = glade_xml_get_widget (glxml, "paste");
	wig->task=NULL;

	glade_xml_signal_connect_data (glxml, "on_new_task_activate",
	        GTK_SIGNAL_FUNC (task_new_task_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_edit_task_activate",
	        GTK_SIGNAL_FUNC (task_edit_task_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_delete_memo_activate",
	        GTK_SIGNAL_FUNC (task_delete_memo_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_delete_times_activate",
	        GTK_SIGNAL_FUNC (task_delete_times_clicked_cb), wig);
	  
	glade_xml_signal_connect_data (glxml, "on_paste_activate",
	        GTK_SIGNAL_FUNC (task_paste_clicked_cb), wig);

	glade_xml_signal_connect_data (glxml, "on_new_interval_activate",
	        GTK_SIGNAL_FUNC (task_new_interval_cb), wig);

	/* ---------------------------------------------------- */
	/* finally ... display the actual journal */

	wig->prj = prj;
	wig->filepath = g_strdup (report);
	if (!prj)
	{
		gtt_ghtml_display (wig->gh, "noproject.ghtml", NULL);
	} 
	else 
	{
		gtt_project_add_notifier (prj, redraw, wig);
		gtt_ghtml_display (wig->gh, report, prj);
	}
}

/* ============================================================== */

static char *
resolve_path (char *path_frag)
{
	const GList *list;
	char buff[PATH_MAX], *path;

	list = gnome_i18n_get_language_list ("LC_MESSAGES");
	for ( ; list; list=list->next) 
	{
		const char *lang = list->data;

		/* See if gtt/ghtml/<lang>/<path_frag> exists */
		/* look in the local build dir first (for testing) */
		
		snprintf (buff, PATH_MAX, "ghtml/%s/%s", lang, path_frag);
		path = gnome_datadir_file (buff);
		if (path) return path;

		snprintf (buff, PATH_MAX, "gtt/ghtml/%s/%s", lang, path_frag);
		path = gnome_datadir_file (buff);
		if (path) return path;

		/* some users compile with path settings that gnome
		 * cannot find.  In that case we have to supply a full
		 * path and check it's existance directly -- we CANNOT
		 * use gnome_datadir_file() because it wont work!
		 *
		 * -warlord 2001-11-29
		 */
		snprintf (buff, PATH_MAX, GTTDATADIR "/ghtml/%s/%s", lang, path_frag);
		if (g_file_exists (buff)) return g_strdup (buff);
	}
	return g_strdup(path_frag);
}

void
edit_journal(GtkWidget *w, gpointer data)
{
	GttProject *prj;
	char * path;

	prj = ctree_get_focus_project (global_ptw);
	if (!prj) return;

	path = resolve_path ("journal.ghtml");
	do_show_report (path, prj);
}

void
edit_alldata(GtkWidget *w, gpointer data)
{
	GttProject *prj;
	char * path;

	prj = ctree_get_focus_project (global_ptw);
	if (!prj) return;

	path = resolve_path ("bigtable.ghtml");
	do_show_report (path, prj);
}

void
edit_invoice(GtkWidget *w, gpointer data)
{
	GttProject *prj;
	char * path;

	prj = ctree_get_focus_project (global_ptw);
	if (!prj) return;

	path = resolve_path ("invoice.ghtml");
	do_show_report (path, prj);
}

void
edit_primer(GtkWidget *w, gpointer data)
{
	GttProject *prj;
	char * path;

	prj = ctree_get_focus_project (global_ptw);
	if (!prj) return;

	path = resolve_path ("primer.ghtml");
	do_show_report (path, prj);
}

void
invoke_report(GtkWidget *widget, gpointer data)
{
	GttProject *prj;
	char * filepath = (char *) data;

	prj = ctree_get_focus_project (global_ptw);
	if (!prj) return;

	/* do not gnome-filepath this, this is for user-defined reports */
	do_show_report (filepath, prj);
}

/* ===================== END OF FILE ==============================  */
