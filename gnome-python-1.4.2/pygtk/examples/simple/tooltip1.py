#!/usr/bin/env python

# translation of "Hello World III" from GTK manual, using gtk_object_new
# with gtkmodule.  Also implements a tooltip for the button.

from _gtk import *
from GTK import *

def hello(*args):
	print "Hello World"
	gtk_widget_destroy(window)

def destroy(*args):
	gtk_widget_hide(window)
	gtk_main_quit()

gtk_init()

tt = gtk_tooltips_new()
gtk_tooltips_set_delay(tt, 500)

window = gtk_object_new(gtk_window_get_type(), {
	'type':         WINDOW_TOPLEVEL,
	'title':        'Hello World',
	'allow_grow':   0,
	'allow_shrink': 0,
	'border_width': 10
})
gtk_signal_connect(window, "destroy", destroy)

button = gtk_object_new(gtk_button_get_type(), {
	'label':   'Hello World',
	'parent':  window,
	'visible': 1
})
gtk_signal_connect(button, "clicked", hello)

gtk_tooltips_set_tip(tt, button, 'Prints "Hello World"', '')
gtk_tooltips_enable(tt)

gtk_widget_show(window)
gtk_main()

