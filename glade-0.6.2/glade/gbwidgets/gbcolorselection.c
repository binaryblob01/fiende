
/*  Gtk+ User Interface Builder
 *  Copyright (C) 1998  Damon Chaplin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <gtk/gtkcolorsel.h>
#include "../gb.h"

/* Include the 21x21 icon pixmap for this widget, to be used in the palette */
#include "../graphics/colorselection.xpm"

/*
 * This is the GbWidget struct for this widget (see ../gbwidget.h).
 * It is initialized in the init() function at the end of this file
 */
static GbWidget gbwidget;

static gchar *Policy = "GtkColorSelection::policy";

static const gchar *GbPolicyChoices[] =
{"Continuous", "Discontinuous", "Delayed",
 NULL};
static const gint GbPolicyValues[] =
{
  GTK_UPDATE_CONTINUOUS,
  GTK_UPDATE_DISCONTINUOUS,
  GTK_UPDATE_DELAYED
};
static const gchar *GbPolicySymbols[] =
{
  "GTK_UPDATE_CONTINUOUS",
  "GTK_UPDATE_DISCONTINUOUS",
  "GTK_UPDATE_DELAYED"
};

/******
 * NOTE: To use these functions you need to uncomment them AND add a pointer
 * to the function in the GbWidget struct at the end of this file.
 ******/

/*
 * Creates a new GtkWidget of class GtkColorSelection, performing any specialized
 * initialization needed for the widget to work correctly in this environment.
 * If a dialog box is used to initialize the widget, return NULL from this
 * function, and call data->callback with your new widget when it is done.
 * If the widget needs a special destroy handler, add a signal here.
 */
/*
   GtkWidget*
   gb_color_selection_new(GbWidgetNewData *data)
   {

   }
 */


/*
 * Creates the components needed to edit the extra properties of this widget.
 */
static void
gb_color_selection_create_properties (GtkWidget * widget, GbWidgetCreateArgData
				      * data)
{
  property_add_choice (Policy, _("Update Policy:"),
		       _("The update policy of the color selection scales"),
		       GbPolicyChoices);
}



/*
 * Gets the properties of the widget. This is used for both displaying the
 * properties in the property editor, and also for saving the properties.
 */
static void
gb_color_selection_get_properties (GtkWidget * widget, GbWidgetGetArgData * data)
{
  gint i;

  for (i = 0; i < sizeof (GbPolicyValues) / sizeof (GbPolicyValues[0]); i++)
    {
      if (GbPolicyValues[i] == GTK_COLOR_SELECTION (widget)->policy)
	gb_widget_output_choice (data, Policy, i, GbPolicySymbols[i]);
    }
}



/*
 * Sets the properties of the widget. This is used for both applying the
 * properties changed in the property editor, and also for loading.
 */
static void
gb_color_selection_set_properties (GtkWidget * widget, GbWidgetSetArgData * data)
{
  gint i;
  gchar *policy;

  policy = gb_widget_input_choice (data, Policy);
  if (data->apply)
    {
      for (i = 0; i < sizeof (GbPolicyValues) / sizeof (GbPolicyValues[0]); i
	   ++)
	{
	  if (!strcmp (policy, GbPolicyChoices[i])
	      || !strcmp (policy, GbPolicySymbols[i]))
	    {
	      gtk_color_selection_set_update_policy (GTK_COLOR_SELECTION (widget),
						     GbPolicyValues[i]);
	      break;
	    }
	}
    }
}



/*
 * Adds menu items to a context menu which is just about to appear!
 * Add commands to aid in editing a GtkColorSelection, with signals pointing to
 * other functions in this file.
 */
/*
   static void
   gb_color_selection_create_popup_menu(GtkWidget *widget, GbWidgetCreateMenuData *data)
   {

   }
 */



/*
 * Writes the source code needed to create this widget.
 * You have to output everything necessary to create the widget here, though
 * there are some convenience functions to help.
 */
static void
gb_color_selection_write_source (GtkWidget * widget, GbWidgetWriteSourceData *
				 data)
{
  gint i;

  if (data->create_widget)
    {
      source_add (data, "  %s = gtk_color_selection_new ();\n", data->wname);
    }

  gb_widget_write_standard_source (widget, data);

  if (GTK_COLOR_SELECTION (widget)->policy != GTK_UPDATE_CONTINUOUS)
    {
      for (i = 0; i < sizeof (GbPolicyValues) / sizeof (GbPolicyValues[0]); i
	   ++)
	{
	  if (GbPolicyValues[i] == GTK_COLOR_SELECTION (widget)->policy)
	    source_add (data, "  gtk_color_selection_set_update_policy (GTK_COLOR_SELECTION (%s), %s);\n",
			data->wname, GbPolicySymbols[i]);
	}
    }
}



/*
 * Initializes the GbWidget structure.
 * I've placed this at the end of the file so we don't have to include
 * declarations of all the functions.
 */
GbWidget *
gb_color_selection_init ()
{
  /* Initialise the GTK type */
  gtk_color_selection_get_type ();

  /* Initialize the GbWidget structure */
  gb_widget_init_struct (&gbwidget);

  /* Fill in the pixmap struct & tooltip */
  gbwidget.pixmap_struct = colorselection_xpm;
  gbwidget.tooltip = _("Color Selection");

  /* Fill in any functions that this GbWidget has */
  gbwidget.gb_widget_create_properties = gb_color_selection_create_properties;
  gbwidget.gb_widget_get_properties = gb_color_selection_get_properties;
  gbwidget.gb_widget_set_properties = gb_color_selection_set_properties;
  gbwidget.gb_widget_write_source = gb_color_selection_write_source;
/*
   gbwidget.gb_widget_new               = gb_color_selection_new;
   gbwidget.gb_widget_create_popup_menu = gb_color_selection_create_popup_menu;
 */

  return &gbwidget;
}
