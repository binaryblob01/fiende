#
# gtk.py: Wrapper for the gtk module.  This one gives an object oriented
#         feel for the library.  Just type "from gtk import *"
#
# Author: James Henstridge <james@daa.com.au>
#
# TODO
# 1. document some of this (maybe all of it)
# 2. add widget members to individual __getattr__()s

# The _obj argument in each constructor is used to create a gtk object
# that coresponds to an already created GTK Object.  It should be possible
# to call any of the gtk object constructors something like this:
# GtkContainer(_obj=gtk_obj) where gtk_obj is an already created GtkObject.
#
# To make the use of this system easier, you can use the _obj2inst function
# to create a gtk object of the correct type from a GtkObject.  This
# is the function the new() command uses.

from GTK import *
import GDK    # make the GDK constants available
import _gtk
import sys, os

# version numbers ...
gtk_version = _gtk.gtk_version
pygtk_version = _gtk.pygtk_version

# this sets up i18n input stuff
_gtk.gtk_set_locale()

# initialise GTK (only if it can find the display -- this avoids segfaults)
if '--display' in sys.argv or os.environ.has_key('DISPLAY'):
	_gtk.gtk_init()
else:
	print 'No display information -- gtk_init not called'
del sys, os

def GdkColor(red, green, blue, pixel=-1):
	return _gtk.gdk_color_new(red, green, blue, pixel)

class GtkObject:
	get_type = _gtk.gtk_object_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def set(self, dict):
		_filtprops(dict)
		_gtk.gtk_object_set(self._o, dict)
	def __getitem__(self, key):
		ret = _gtk.gtk_object_get(self._o, key)
		if type(ret) == _gtk.GtkObjectType:
			return _obj2inst(ret)
		else:
			return ret
	def __setitem__(self, key, v):
		if hasattr(v, '_o') and type(v._o) == _gtk.GtkObjectType:
			v = v._o
		elif hasattr(v, '_ag') and type(v._ag)==_gtk.GtkAccelGroupType:
			v = v._ag
		elif hasattr(v, '_im'):
			v = v._im
		_gtk.gtk_object_set(self._o, {key: v});
	def __cmp__(self, other):
	        if hasattr(other, '_o'):
			return cmp(self._o, other._o)
		else:
			return cmp(id(self), id(other))
	def __hash__(self):
		return hash(self._o)
	def __getattr__(self, attr):
		# this function allows setting attributes on an object so that
		# they will always be available with the object.  Due to
		# reference counting problems, we can't always pass the
		# same GtkObject instance to a callback.
		#if attr[0] == '_' or not self.__dict__.has_key('_o'):
		#	raise AttributeError, attr
		#dict = self.get_data('Python-Attributes')
		#if dict and dict.has_key(attr):
		#	return dict[attr]
		raise AttributeError, attr
	#def __setattr__(self, attr, value):
	#	if attr[0] == '_' or self.__dict__.has_key(attr) or \
	#	   not self.__dict__.has_key('_o'):
	#		self.__dict__[attr] = value
	#	dict = self.get_data('Python-Attributes')
	#	if not dict:
	#		dict = {}
	#		self.set_data('Python-Attributes', dict)
	#	dict[attr] = value
	#def __delattr__(self, attr):
	#	if self.__dict__.has_key(attr):
	#		del self.__dict__[attr]
	#		return
	#	if not self.__dict__.has_key('_o'):
	#		raise AttributeError, \
	#		      'delete non-existing instance attribute'
	#	dict = self.get_data('Python-Attributes')
	#	if dict and dict.has_key(attr):
	#		del dict[attr]
	#	else:
	#		raise AttributeError, \
	#		      'delete non-existing instance attribute'
			
	def flags(self, mask=None):
		if mask:
			return _gtk.GTK_OBJECT_FLAGS(self._o) & mask
		else:
			return _gtk.GTK_OBJECT_FLAGS(self._o)
	def set_flags(self, flags):
		_gtk.GTK_OBJECT_SET_FLAGS(self._o, flags)
	def unset_flags(self, flags):
		_gtk.GTK_OBJECT_UNSET_FLAGS(self._o, flags)
	def set_data(self, key, value):
		_gtk.gtk_object_set_data(self._o, key, value)
	def get_data(self, key):
		return _gtk.gtk_object_get_data(self._o, key)
	def remove_data(self, key):
		_gtk.gtk_object_remove_data(self._o, key)
	class __cnv:
		def __init__(self, func):
			self.func = func
		def __call__(self, *args):
			a = list(args)
			for i in range(len(args)):
				if type(args[i]) == _gtk.GtkObjectType:
					a[i] = _obj2inst(args[i])
				elif type(args[i]) == _gtk.GtkAccelGroupType:
					a[i] = GtkAccelGroup(_obj=args[i])
			a = tuple(a)
			ret = apply(self.func, a)
			if hasattr(ret, '_o'): ret = ret._o
			elif hasattr(ret, '_ag'): ret = ret._ag
			elif hasattr(ret, '_im'): ret = ret._im
			return ret
	def connect(self, name, f, *extra):
		callback = self.__cnv(f)
		return _gtk.gtk_signal_connect(self._o, name,
						   callback.__call__, extra)
	signal_connect = connect
	def connect_after(self, name, f, *extra):
		callback = self.__cnv(f)
		return _gtk.gtk_signal_connect_after(self._o, name,
						     callback.__call__, extra)
	signal_connect_after = connect_after
	def connect_object(self, name, f, obj, *extra):
		callback = self.__cnv(f)
		return _gtk.gtk_signal_connect_object(self._o, name,
						      callback.__call__,
						      obj._o, extra)
	signal_connect_object = connect_object
	def connect_object_after(self, name, f, obj, *extra):
		callback = self.__cnv(f)
		return _gtk.gtk_signal_connect_object_after(self._o, name,
							    callback.__call__,
							    obj._o, extra)
	signal_connect_after = connect_after
	def disconnect(self, id):
		_gtk.gtk_signal_disconnect(self._o, id)
	def signal_handler_block(self, id):
		_gtk.gtk_signal_handler_block(self._o, id)
	def signal_handler_unblock(self, id):
		_gtk.gtk_signal_handler_unblock(self._o, id)
	def signal_handlers_destroy(self):
		_gtk.gtk_signal_handlers_destroy(self._o)
	def emit(self, signal, *args):
		params = []
		for i in args:
			if hasattr(i, '_o'): params.append(i._o)
			elif hasattr(i, '_ag'): params.append(i._ag)
			elif hasattr(i, '_im'): params.append(i._im)
			else: params.append(i)
		return _gtk.gtk_signal_emitv_by_name(self._o,signal,params)
	def emit_stop_by_name(self, sig):
		_gtk.gtk_signal_emit_stop_by_name(self._o, sig)
	def destroy(self, _obj=None):
		_gtk.gtk_object_destroy(self._o)

class GtkData(GtkObject):
	get_type = _gtk.gtk_data_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return

class GtkAdjustment(GtkData):
	get_type = _gtk.gtk_adjustment_get_type
	def __init__(self, value=0, lower=0, upper=0, step_incr=0, page_incr=0,
			page_size=0, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_adjustment_new(value, lower, upper,
					  step_incr, page_incr, page_size)
	def __getattr__(self, attr):
		attrs = {
		'lower': _gtk.gtk_adjustment_get_lower,
		'upper': _gtk.gtk_adjustment_get_upper,
		'value': _gtk.gtk_adjustment_get_value,
		'step_increment': _gtk.gtk_adjustment_get_step_increment,
		'page_increment': _gtk.gtk_adjustment_get_page_increment,
		'page_size': _gtk.gtk_adjustment_get_page_size
		}
		if attrs.has_key(attr):
			return attrs[attr](self._o)
		return GtkData.__getattr__(self, attr)
	def set_value(self, value):
		_gtk.gtk_adjustment_set_value(self._o, value)
	def set_all(self, value, lower, upper, step_increment, page_increment,
		    page_size):
		_gtk.gtk_adjustment_set_all(self._o, value, lower, upper,
					    step_increment, page_increment,
					    page_size)
	def clamp_page(self, lower, upper):
		_gtk.gtk_adjustment_clamp_page(self._o, lower, upper)
	def changed(self, obj=None):
		_gtk.gtk_adjustment_changed(self._o)
	def value_changed(self, obj=None):
		_gtk.gtk_adjustment_value_changed(self._o)

class GtkTooltips(GtkData):
	get_type = _gtk.gtk_tooltips_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_tooltips_new()
	def enable(self):
		_gtk.gtk_tooltips_enable(self._o)
	def disable(self, obj=None):
		_gtk.gtk_tooltips_disable(self._o)
	def set_delay(self, delay):
		_gtk.gtk_tooltips_set_delay(self._o, delay)
	def set_tip(self, w, tip, tip_private=None):
		_gtk.gtk_tooltips_set_tip(self._o, w._o, tip, tip_private)
	def set_tips(self, w, tip):
		print "GtkTooltip.set_tips deprecated -- use set_tip instead"
		self.set_tip(w, tip, tip)
	def set_colors(self, back, fore):
		_gtk.gtk_tooltips_set_colors(self._o, back, fore)
	def force_window(self):
		_gtk.gtk_tooltips_force_window(self._o)

class GtkItemFactory(GtkObject):
	get_type = _gtk.gtk_item_factory_get_type
	def __init__(self, type=-1, path='', accel_group=None, _obj=None):
		if _obj: self._o = _obj; return
		if type == -1: type = _gtk.gtk_menu_bar_get_type()
		if hasattr(type, 'get_type'): type = type.get_type()
		self._o = _gtk.gtk_item_factory_new(type,path,accel_group._ag)
	parse_rc = _gtk.gtk_item_factory_parse_rc
	parse_rc_string = _gtk.gtk_item_factory_parse_rc_string
	def get_widget(self, path):
		ret = _gtk.gtk_item_factory_get_widget(self._o, path)
		if ret:
			return _obj2inst(ret)
		else:
			return ret
	def get_widget_by_action(self, action):
		ret = _gtk.gtk_item_factory_get_widget_by_action(self._o,
								 action)
		if ret:
			return _obj2inst(ret)
		else:
			return ret
	class __wrap_cb:
		def __init__(self, cb):
			self.cb = cb
		def __call__(self, action, widget):
			self.cb(action, _obj2inst(widget))
	def create_items(self, items):
		def map_func((path,accel,cb,action,type), wrap=self.__wrap_cb):
			if cb:
				return (path,accel,wrap(cb).__call__,action,
					type)
			else:
				return (path,accel,None,action,type)
		items = map(map_func, items)
		_gtk.gtk_item_factory_create_items(self._o, items)
	def delete_item(self, path):
		_gtk.gtk_item_factory_delete_item(self._o, path)
	def popup(self, x, y, button, time):
		_gtk.gtk_item_factory_popup(self._o, x, y, button, time)
	def add_foreign(self, accel_widget, full_path, accel_group, keyval,
			modifiers):
		_gtk.gtk_item_factory_add_foreign(accel_widget._o, full_path,
						  accel_group._ag, keyval,
						  modifiers)

class GtkWidget(GtkObject):
	get_type = _gtk.gtk_widget_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	# extra parameter is so that these funcs can be used as signal funcs
	def accelerators_locked(self):
		return _gtk.gtk_widget_accelerators_locked(self._o)
	def activate(self, obj=None):
		return _gtk.gtk_widget_activate(self._o)
	def add_accelerator(self, signal, group, key, mods, flags):
		_gtk.gtk_widget_add_accelerator(self._o, signal, group._ag,
						key, mods, flags)
	def destroy(self, obj=None):
		_gtk.gtk_widget_destroy(self._o)
	def drag_get_data(self, context, target, time):
		_gtk.gtk_drag_get_data(self._o, context, target, time)
	def drag_finish(self, context, success, delete, time):
		_gtk.gtk_drag_finish(context, success, delete, time)
	def drag_highlight(self):
		_gtk.gtk_drag_highlight(self._o)
	def drag_unhighlight(self):
		_gtk.gtk_drag_unhighlight(self._o)
	# targets is a list of 3-tuples of form
	# (string target, int flags ,int info)
	def drag_dest_set(self, flags, targets, actions):
		_gtk.gtk_drag_dest_set(self._o, flags, targets, actions)
	def drag_dest_set_proxy(self, proxy_window, protocol, use_coordinates):
		_gtk.gtk_drag_dest_set_proxy(self._o, proxy_window, protocol,
					     use_coordinates)
	def drag_dest_unset(self):
		_gtk.gtk_drag_dest_unset(self._o)
	# targets as for drag_dest_set
	def drag_source_set(self, button_mask, targets, actions):
		_gtk.gtk_drag_source_set(self._o, button_mask, targets,actions)
	def drag_source_set_icon(self, colormap, pixmap, mask):
		_gtk.gtk_drag_source_set_icon(self._o, colormap, pixmap, mask)
	# targets as for drag_dest_set
	def drag_begin(self, targets, actions, button, event):
		return _gtk.gtk_drag_begin(self._o, targets, actions, button,
					   event)
	def drag_set_icon_widget(self, context, widget, hot_x, hot_y):
		_gtk.gtk_drag_set_icon_widget(context, widget._o, hot_x, hot_y)
	def drag_set_icon_pixmap(self, context, colormap, pixmap, mask,
				 hot_x, hot_y):
		_gtk.gtk_drag_set_icon_pixmap(context, colormap, pixmap, mask,
					      hot_x, hot_y)
	def drag_set_icon_default(self, context):
		_gtk.gtk_drag_set_icon_default(context)
	def drag_set_default_icon(self, colormap, pixmap, mask, hot_x, hot_y):
		_gtk.gtk_drag_set_default_icon(colormap, pixmap, mask,
					       hot_x, hot_y)
	def drag_get_source_widget(self, context):
		widget = _gtk.gtk_drag_get_source_widget(context)
		if widget:
			return _obj2inst(widget)
		return None
	def drag_status(self, context, action, time):
		_gtk.gdk_drag_status(context, action, time)
	def draw(self, rect):
		_gtk.gtk_widget_draw(self._o, rect)
	def draw_default(self, obj=None):
		_gtk.gtk_widget_draw_default(self._o)
	def draw_focus(self, obj=None):
		_gtk.gtk_widget_draw_focus(self._o)
	def ensure_style(self):
		_gtk.gtk_widget_ensure_style(self._o)
	def event(self, event):
		_gtk.gtk_widget_event(self._o, event)
	def lock_accelerators(self):
		_gtk.gtk_widget_lock_accelerators(self._o)
	def get_allocation(self):
		return _gtk.gtk_widget_get_allocation(self._o)
	def get_ancestor(self, type):
		return _obj2inst(_gtk.gtk_widget_get_ancestor(self._o, type))
	def get_child_requisition(self):
		return _gtk.gtk_widget_get_child_requisition(self._o)
	def get_colormap(self):
		return _gtk.gtk_widget_get_colormap(self._o)
	def get_composite_name(self):
		return _gtk.gtk_widget_get_composite_name(self._o)
	def get_events(self):
		return _gtk.gtk_widget_get_events(self._o)
	def get_extension_events(self):
		return _gtk.gtk_widget_get_extension_events(self._o)
	def get_name(self):
		return _gtk.gtk_widget_get_name(self._o)
	def get_parent_window(self):
		return _gtk.gtk_widget_get_parent_window(self._o)
	def get_pointer(self):
		return _gtk.gtk_widget_get_pointer(self._o)
	def get_style(self):
		return _gtk.gtk_widget_get_style(self._o)
	def get_toplevel(self):
		return _obj2inst(_gtk.gtk_widget_get_toplevel(self._o))
	def get_window(self):
		return _gtk.gtk_widget_get_window(self._o)
	def grab_focus(self, obj=None):
		_gtk.gtk_widget_grab_focus(self._o)
	def grab_default(self, obj=None):
		_gtk.gtk_widget_grab_default(self._o)
	def hide(self, obj=None):
		_gtk.gtk_widget_hide(self._o)
	def hide_all(self, obj=None):
		_gtk.gtk_widget_hide_all(self._o)
	def hide_on_delete(self, obj=None):
		return _gtk.gtk_widget_hide_on_delete(self._o)
	def intersect(self, rect):
		return _gtk.gtk_widget_intersect(self._o, rect)
	def is_ancestor(self, a):
		return _gtk.gtk_widget_is_ancestor(self._o, a._o)
	def map(self, obj=None):
		_gtk.gtk_widget_map(self._o)
	def popup(self, x, y):
		_gtk.gtk_widget_popup(self._o, x, y)
	def queue_draw(self, obj=None):
		_gtk.gtk_widget_queue_draw(self._o)
	def queue_draw_area(self, x, y, width, height):
		_gtk.gtk_widget_queue_draw(self._o, x, y, width, height)
	def queue_clear(self, obj=None):
		_gtk.gtk_widget_queue_draw(self._o)
	def queue_clear_area(self, x, y, width, height):
		_gtk.gtk_widget_queue_draw(self._o, x, y, width, height)
	def queue_resize(self, obj=None):
		_gtk.gtk_widget_queue_resize(self._o)
	def realize(self, obj=None):
		_gtk.gtk_widget_realize(self._o)
	def remove_accelerator(self, group, key, mods):
		_gtk.gtk_widget_remove_accelerator(self._o, group._ag,
						   key, mods)
	def remove_accelerators(self, signal, visible_only):
		_gtk.gtk_widget_remove_accelerators(self._o, signal,
						    visible_only)
	def reparent(self, new_parent):
		_gtk.gtk_widget_reparent(self._o, new_parent._o)
	def reset_rc_styles(self):
		_gtk.gtk_widget_reset_rc_styles(self._o)
	def restore_default_style(self):
		_gtk.gtk_widget_restore_default_style(self._o)
	def selection_owner_set(self, selection, time):
		return _gtk.gtk_selection_owner_set(self._o, selection, time)
	def selection_add_target(self, selection, target, info):
		_gtk.gtk_selection_add_target(self._o, selection, target, info)
	def selection_convert(self, selection, target, time):
		return _gtk.gtk_selection_convert(self._o, selection,
						  target, time)
	def selection_remove_all(self):
		_gtk.gtk_selection_remove_all(self._o)
	def set_events(self, events):
		_gtk.gtk_widget_set_events(self._o, events)
	def add_events(self, events):
		_gtk.gtk_widget_add_events(self._o, events)
	def set_app_paintable(self, app_paintable):
		_gtk.gtk_widget_set_app_paintable(self._o, app_paintable)
	def set_composite_name(self, name):
		_gtk.gtk_widget_set_composite_name(self._o, name)
	def set_extension_events(self, exevents):
		_gtk.gtk_widget_set_extension_events(self._o, exevents)
	def set_name(self, name):
		_gtk.gtk_widget_set_name(self._o, name)
	def set_parent(self, parent):
		_gtk.gtk_widget_set_parent(self._o, parent._o)
	def set_parent_window(self, parent):
		_gtk.gtk_widget_set_parent_window(self._o, parent)
	def set_scroll_adjustments(self, hadj, vadj):
		return _gtk.gtk_widget_set_scroll_adjustments(self._o, hadj._o,
							      vadj._o)
	def set_sensitive(self, s):
		_gtk.gtk_widget_set_sensitive(self._o, s)
	def set_state(self, s):
		_gtk.gtk_widget_set_state(self._o, s)
	def set_style(self, style):
		_gtk.gtk_widget_set_style(self._o, style)
	def set_rc_style(self):
		_gtk.gtk_widget_set_rc_style(self._o)
	def set_uposition(self, x, y):
		_gtk.gtk_widget_set_uposition(self._o, x, y)
	def set_usize(self, w, h):
		_gtk.gtk_widget_set_usize(self._o, w, h)
	def shape_combine_mask(self, mask, ofs_x, ofs_y):
		if hasattr(mask, '_o'):
			tmp, mask = mask.get()
	        _gtk.gtk_widget_shape_combine_mask(self._o, mask, ofs_x, ofs_y)
	def show(self, obj=None):
		_gtk.gtk_widget_show(self._o)
	def show_all(self, obj=None):
		_gtk.gtk_widget_show_all(self._o)
	def show_now(self, obj=None):
	        _gtk.gtk_widget_show_now(self._o)
	def size_allocate(self, a):
		_gtk.gtk_widget_size_allocate(self._o, a)
	def size_request(self):
		return _gtk.gtk_widget_size_request(self._o)
	def unlock_accelerators(self):
		_gtk.gtk_widget_unlock_accelerators(self._o)
	def unmap(self, obj=None):
		_gtk.gtk_widget_unmap(self._o)
	def unparent(self):
		_gtk.gtk_widget_unparent(self._o)
	def unrealize(self, obj=None):
		_gtk.gtk_widget_unrealize(self._o)

class GtkCalendar(GtkWidget):
	get_type = _gtk.gtk_calendar_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_calendar_new()
	def select_month(self, month, year):
		return _gtk.gtk_calendar_select_month(self._o, month, year)
	def select_day(self, day):
		_gtk.gtk_calendar_select_day(self._o, day)
	def mark_day(self, day):
		return _gtk.gtk_calendar_mark_day(self._o, day)
	def unmark_day(self, day):
		return _gtk.gtk_calendar_unmark_day(self._o, day)
	def clear_marks(self):
		_gtk.gtk_calendar_clear_marks(self._o)
	def display_options(self, flags):
		_gtk.gtk_calendar_display_options(self._o, flags)
	def get_date(self):
		return _gtk.gtk_calendar_get_date(self._o)
	def freeze(self):
		_gtk.gtk_calendar_freeze(self._o)
	def thaw(self):
		_gtk.gtk_calendar_thaw(self._o)
	
class GtkContainer(GtkWidget):
	get_type = _gtk.gtk_container_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def add(self, child, **args):
		if args == {}:
			_gtk.gtk_container_add(self._o, child._o)
		else:
			_gtk.gtk_container_add(self._o, child._o, args)
	def border_width(self, bw):
		print "border_width deprecated -- use set_border_width"
		self.set_border_width(bw)
	def set_border_width(self, bw):
		_gtk.gtk_container_set_border_width(self._o, bw)
	def check_resize(self):
		_gtk.gtk_container_check_resize(self._o)
	def child_composite_name(self, child):
		return _gtk.gtk_container_child_composite_name(self._o,
							       child._o)
	def child_set(self, child, **args):
		_gtk.gtk_container_child_set(self._o, child._o, args)
	def child_get(self, child, arg):
		return _gtk.gtk_container_child_get(self._o, child._o, arg)
	def children(self):
		l =  _gtk.gtk_container_children(self._o)
		return map(_obj2inst, l)
	def child_type(self):
		return _gtk.gtk_container_child_type(self._o)
	def focus(self, d):
		_gtk.gtk_container_focus(self._o, d)
	def foreach(self, f):
		for child in self.children():
			f(child)
	def register_toplevel(self):
		_gtk.gtk_container_register_toplevel(self._o)
	def remove(self, child):
		_gtk.gtk_container_remove(self._o, child._o)
	def resize_children(self):
		_gtk.gtk_container_resize_children(self._o)
	def set_focus_child(self, child):
		if child: child = child._o
		_gtk.gtk_container_set_focus_child(self._o, child)
	def set_focus_vadjustment(self, adj):
		_gtk.gtk_container_set_focus_vadjustment(self._o, adj._o)
	def set_focus_hadjustment(self, adj):
		_gtk.gtk_container_set_focus_hadjustment(self._o, adj._o)
	def set_resize_mode(self, mode):
		_gtk.gtk_container_set_resize_mode(self._o, mode)
	def unregister_toplevel(self):
		_gtk.gtk_container_unregister_toplevel(self._o)

class GtkBin(GtkContainer):
	get_type = _gtk.gtk_bin_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return

class GtkAlignment(GtkBin):
	get_type = _gtk.gtk_alignment_get_type
	def __init__(self, xa=0, ya=0, xs=0, ys=0, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_alignment_new(xa, ya, xs, ys)
	def set(self, xa, ya, xs, ys):
		_gtk.gtk_alignment_set(self._o, xa, ya, xs, ys)

class GtkButton(GtkBin):
	get_type = _gtk.gtk_button_get_type
	def __init__(self, label=None, _obj=None):
		if _obj: self._o = _obj; return
		if label == None:
			self._o = _gtk.gtk_button_new()
		else:
			self._o = _gtk.gtk_button_new_with_label(label)
	def pressed(self, obj=None):
		_gtk.gtk_button_pressed(self._o)
	def released(self, obj=None):
		_gtk.gtk_button_released(self._o)
	def clicked(self, obj=None):
		_gtk.gtk_button_clicked(self._o)
	def enter(self, obj=None):
		_gtk.gtk_button_enter(self._o)
	def leave(self, obj=None):
		_gtk.gtk_button_leave(self._o)
	def set_relief(self, style):
		_gtk.gtk_button_set_relief(self._o, style)
	def get_relief(self):
		return _gtk.gtk_button_get_relief(self._o)

class GtkOptionMenu(GtkButton):
	get_type = _gtk.gtk_option_menu_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_option_menu_new()
	def get_menu(self):
		return _obj2inst(_gtk.gtk_option_menu_get_menu(self._o))
	def set_menu(self, menu):
		_gtk.gtk_option_menu_set_menu(self._o, menu._o)
	def remove_menu(self):
		_gtk.gtk_option_menu_remove_menu(self._o)
	def set_history(self, index):
		_gtk.gtk_option_menu_set_history(self._o, index)

class GtkToggleButton(GtkButton):
	get_type = _gtk.gtk_toggle_button_get_type
	def __init__(self, label=None, _obj=None):
		if _obj: self._o = _obj; return
		if label == None:
			self._o = _gtk.gtk_toggle_button_new()
		else:
			self._o = _gtk.gtk_toggle_button_new_with_label(label)
	def __getattr__(self, attr):
	    attrs = {
		    'active': _gtk.gtk_toggle_button_get_active,
		    'draw_indicator':_gtk.gtk_toggle_button_get_draw_indicator,
	    }
	    if attrs.has_key(attr):
		    return attrs[attr](self._o)
	    return GtkButton.__getattr__(self, attr)
	def set_mode(self, di):
		_gtk.gtk_toggle_button_set_mode(self._o, di)
	def set_active(self, active):
		_gtk.gtk_toggle_button_set_active(self._o, active)
	def get_active(self):
		return _gtk.gtk_toggle_button_get_active(self._o)
	def toggled(self, obj=None):
		_gtk.gtk_toggle_button_toggled(self._o)

class GtkCheckButton(GtkToggleButton):
	get_type = _gtk.gtk_check_button_get_type
	def __init__(self, label=None, _obj=None):
		if _obj: self._o = _obj; return
		if label == None:
			self._o = _gtk.gtk_check_button_new()
		else:
			self._o = _gtk.gtk_check_button_new_with_label(label)

class GtkRadioButton(GtkCheckButton):
	get_type = _gtk.gtk_radio_button_get_type
	def __init__(self, group=None, label=None, _obj=None):
		if _obj: self._o = _obj; return
		if label == None:
			if group == None:
				self._o = _gtk.gtk_radio_button_new()
			else:
				self._o = \
					_gtk.gtk_radio_button_new_from_widget(
						group._o)
		else:
			if group == None:
				self._o = _gtk.gtk_radio_button_new_with_label(
					None, label)
			else:
				self._o = _gtk.gtk_radio_button_new_with_label_from_widget(group._o, label)

class GtkFrame(GtkBin):
	get_type = _gtk.gtk_frame_get_type
	def __init__(self, label=None, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_frame_new(label)
	def set_label(self, label):
		_gtk.gtk_frame_set_label(self._o, label)
	def set_label_align(self, x, y):
		_gtk.gtk_frame_set_label_align(self._o, x, y)
	def set_shadow_type(self, tp=SHADOW_ETCHED_IN):
		_gtk.gtk_frame_set_shadow_type(self._o, tp)

class GtkAspectFrame(GtkFrame):
	get_type = _gtk.gtk_aspect_frame_get_type
	def __init__(self, label=None, xa=0, ya=0, ratio=1, obey_child=1,
		     _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_aspect_frame_new(label, xa, ya, ratio,
						    obey_child)
	def set(self, xa, ya, ratio, obey_child):
		_gtk.gtk_aspect_frame_set(self._o, xa, ya, ratio, obey_child)

class GtkEventBox(GtkBin):
	get_type = _gtk.gtk_event_box_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_event_box_new()

class GtkHandleBox(GtkBin):
	get_type = _gtk.gtk_handle_box_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_handle_box_new()
	def set_shadow_type(self, type):
		_gtk.gtk_handle_box_set_shadow_type(self._o, type)
	def set_handle_position(self, position):
		_gtk.gtk_handle_box_set_handle_position(self._o, position)
	def set_snap_edge(self, edge):
		_gtk.gtk_handle_box_set_snap_edge(self._o, edge)

class GtkItem(GtkBin):
	get_type = _gtk.gtk_item_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def select(self, obj=None):
		_gtk.gtk_item_select(self._o)
	def deselect(self, obj=None):
		_gtk.gtk_item_deselect(self._o)
	def toggle(self, obj=None):
		_gtk.gtk_item_toggle(self._o)

class GtkMenuItem(GtkItem):
	get_type = _gtk.gtk_menu_item_get_type
	def __init__(self, label=None, _obj=None):
		if _obj: self._o = _obj; return
		if label == None:
			self._o = _gtk.gtk_menu_item_new()
		else:
			self._o = _gtk.gtk_menu_item_new_with_label(label)
	def get_submenu(self):
		submenu = _gtk.gtk_menu_item_get_submenu(self._o)
		if submenu:
			return _obj2inst(submenu)
		return None
	def set_submenu(self, sub):
		_gtk.gtk_menu_item_set_submenu(self._o, sub._o)
	def set_placement(self, placement):
		_gtk.gtk_menu_item_set_placement(self._o, placement)
	def configure(self, show_toggle, show_sub):
		_gtk.gtk_menu_item_configure(self._o, show_toggle, show_sub)
	def select(self, obj=None):
		_gtk.gtk_menu_item_select(self._o)
	def deselect(self, obj=None):
		_gtk.gtk_menu_item_deselect(self._o)
	def activate(self, obj=None):
		_gtk.gtk_menu_item_activate(self._o)
	def remove_submenu(self):
		_gtk.gtk_menu_item_remove_submenu(self._o)
	def right_justify(self, obj=None):
		_gtk.gtk_menu_item_right_justify(self._o)
		
class GtkCheckMenuItem(GtkMenuItem):
	get_type = _gtk.gtk_check_menu_item_get_type
	def __init__(self, label=None, _obj=None):
		if _obj: self._o = _obj; return
		if label == None:
			self._o = _gtk.gtk_check_menu_item_new()
		else:
			self._o =_gtk.gtk_check_menu_item_new_with_label(label)
	def __getattr__(self, attr):
		attrs = {
			'active': _gtk.gtk_check_menu_item_get_active
		}
		if attrs.has_key(attr):
			return attrs[attr](self._o)
		return GtkMenuItem.__getattr__(self, attr)
	def set_show_toggle(self, always):
		_gtk.gtk_check_menu_item_set_show_toggle(self._o, always)
	def set_active(self, active):
		_gtk.gtk_check_menu_item_set_active(self._o, active)
	def toggled(self, obj=None):
		_gtk.gtk_check_menu_item_toggled(self._o)

class GtkRadioMenuItem(GtkCheckMenuItem):
	get_type = _gtk.gtk_radio_menu_item_get_type
	def __init__(self, group=None, label=None, _obj=None):
		if _obj: self._o = _obj; return
		if label == None:
			if group == None:
				self._o = _gtk.gtk_radio_menu_item_new()
			else:
				self._o =_gtk.gtk_radio_menu_item_new(group._o)
		else:
			if group == None:
				self._o=_gtk.gtk_radio_menu_item_new_with_label(
					None, label)
			else:
				self._o=_gtk.gtk_radio_menu_item_new_with_label(
					group._o, label)

class GtkTearoffMenuItem(GtkMenuItem):
	get_type = _gtk.gtk_tearoff_menu_item_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_tearoff_menu_item_new()

class GtkListItem(GtkItem):
	get_type = _gtk.gtk_list_item_get_type
	def __init__(self, label=None, _obj=None):
		if _obj: self._o = _obj; return
		if label == None:
			self._o = _gtk.gtk_list_item_new()
		else:
			self._o = _gtk.gtk_list_item_new_with_label(label)
	def select(self, obj=None):
		_gtk.gtk_list_item_select(self._o)
	def deselect(self, obj=None):
		_gtk.gtk_list_item_deselect(self._o)

class GtkTreeItem(GtkItem):
	get_type = _gtk.gtk_tree_item_get_type
	def __init__(self, label=None, _obj=None):
		if _obj: self._o = _obj; return
		if label == None:
			self._o = _gtk.gtk_tree_item_new()
		else:
			self._o = _gtk.gtk_tree_item_new_with_label(label)
	def __getattr__(self, attr):
		def subtree_wrap(obj):
			o = _gtk.gtk_tree_item_get_subtree(obj)
			if o == None: return None
			return _obj2inst(o)
		attrs = {
			'subtree': subtree_wrap,
			}
		if attrs.has_key(attr):
			return attrs[attr](self._o)
		return GtkItem.__getattr__(self, attr)
	def set_subtree(self, subtree):
		_gtk.gtk_tree_item_set_subtree(self._o, subtree._o)
	def select(self, obj=None):
		_gtk.gtk_tree_item_select(self._o)
	def deselect(self, obj=None):
		_gtk.gtk_tree_item_deselect(self._o)
	def expand(self, obj=None):
		_gtk.gtk_tree_item_expand(self._o)
	def collapse(self, obj=None):
		_gtk.gtk_tree_item_collapse(self._o)
	def remove_subtree(self):
		_gtk.gtk_tree_item_remove_subtree(self._o)

class GtkViewport(GtkBin):
	get_type = _gtk.gtk_viewport_get_type
	def __init__(self, ha=None, va=None, _obj=None):
		if _obj: self._o = _obj; return
		if ha: ha = ha._o
		if va: va = va._o
		self._o = _gtk.gtk_viewport_new(ha, va)
	def get_hadjustment(self):
		return _obj2inst(_gtk.gtk_viewport_get_hadjustment(self._o))
	def get_vadjustment(self):
		return _obj2inst(_gtk.gtk_viewport_get_vadjustment(self._o))
	def set_hadjustment(self, ha):
		_gtk.gtk_viewport_set_hadjustment(self._o, ha._o)
	def set_vadjustment(self, va):
		_gtk.gtk_viewport_set_vadjustment(self._o, va._o)
	def set_shadow_type(self, tp=SHADOW_ETCHED_IN):
		_gtk.gtk_viewport_set_shadow_type(self._o, tp)

class GtkWindow(GtkBin):
	get_type = _gtk.gtk_window_get_type
	def __init__(self, type=WINDOW_TOPLEVEL, title=None, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_window_new(type)
		if title is not None:
			self.set_title(title)
	def __getattr__(self, attr): 
		attrs = {
			'focus_widget' : _gtk.gtk_window_get_focus_widget,
			'default_widget' : _gtk.gtk_window_get_default_widget,
			'transient_parent' : _gtk.gtk_window_get_transient_parent,
			'resize_count': _gtk.gtk_window_get_resize_count,
			'allow_shrink': _gtk.gtk_window_get_allow_shrink,
			'allow_grow': _gtk.gtk_window_get_allow_grow,
			'auto_shrink': _gtk.gtk_window_get_auto_shrink,
			'handling_resize': _gtk.gtk_window_get_handling_resize,
			# We already have a position method 
			'window_position': _gtk.gtk_window_get_position,
			'use_uposition' :  _gtk.gtk_window_get_use_uposition,
			'modal':  _gtk.gtk_window_get_modal,
			'window_has_focus' :  _gtk.gtk_window_get_window_has_focus,
			'window_has_pointer_focus' :
				_gtk.gtk_window_get_window_has_pointer_focus
        }
		if attrs.has_key(attr):
			ret = attrs[attr](self._o)
			# avoid spurious None checks
			if ret is None:
				return None
			if type(ret) == _gtk.GtkObjectType:
				return _obj2inst(ret)
			else:
				return ret
		#else:
		#	ret = _gtk.gtk_object_get(self._o, key)
		#	if type(ret) == _gtk.GtkObjectType:
		#		return _obj2inst(ret)
		#	else:
		#		return ret
		return GtkBin.__getattr__(self, attr)
	def activate_focus(self, obj=None):
		_gtk.gtk_window_activate_focus(self._o)
	def activate_default(self, obj=None):
		_gtk.gtk_window_activate_default(self._o)
	def set_title(self, title):
		_gtk.gtk_window_set_title(self._o, title)
	def set_policy(self, as, ag, autos):
		_gtk.gtk_window_set_policy(self._o, as, ag, autos)
	def add_accel_group(self, group):
		_gtk.gtk_window_add_accel_group(self._o, group._ag)
	def remove_accel_group(self, group):
		_gtk.gtk_window_remove_accel_group(self._o, group._ag)
	def position(self, pos):
		print "position deprecated -- use set_position"
		self.set_position(pos)
	def set_position(self, pos):
		_gtk.gtk_window_set_position(self._o, pos)
	def set_wmclass(self, wmc_name, wmc_class):
		_gtk.gtk_window_set_wmclass(self._o, wmc_name, wmc_class)
	def set_icon(self, pixmap, mask=None):
		if not mask and hasattr(pixmap, '_o'):
			pixmap, mask = pixmap.get()
		_gtk.gtk_window_set_icon(self._o, pixmap, mask)
	def set_icon_name(self, name):
		_gtk.gtk_window_set_icon_name(self._o, name)
	def set_transient_for(self, parent):
		_gtk.gtk_window_set_transient_for(self._o, parent._o)
	def set_geometry_hints(self, geometry_widget=None, **hints):
		# acceptable hints are min_width, min_height, max_width,
		# max_height, base_width, base_height, width_inc, height_inc
		# (with int values) and min_aspect, max_aspect (double values).
		if hasattr(geometry_widget, '_o'):
			geometry_widget = geometry_widget._o
		_gtk.gtk_window_set_geometry_hints(self._o, geometry_widget,
						   hints)
	def set_default_size(self, width, height):
		_gtk.gtk_window_set_default_size(self._o, width, height)
	def set_modal(self, modal):
		_gtk.gtk_window_set_modal(self._o, modal)

class GtkColorSelectionDialog(GtkWindow):
	get_type = _gtk.gtk_color_selection_dialog_get_type
	def __init__(self, name="", _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_color_selection_dialog_new(name)
	def __getattr__(self, attr):
		attrs = {
			'colorsel':_gtk.gtk_color_selection_dialog_get_colorsel,
			'main_vbox':_gtk.gtk_color_selection_dialog_get_main_vbox,
			'ok_button': _gtk.gtk_color_selection_dialog_get_ok_button,
			'reset_button': _gtk.gtk_color_selection_dialog_get_reset_button,
			'cancel_button': _gtk.gtk_color_selection_dialog_get_cancel_button,
			'help_button': _gtk.gtk_color_selection_dialog_get_help_button
		}
		if attrs.has_key(attr):
			return _obj2inst(attrs[attr](self._o))
		return GtkWindow.__getattr__(self, attr)

class GtkDialog(GtkWindow):
	get_type = _gtk.gtk_dialog_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_dialog_new()
	def __getattr__(self, attr):
		attrs = {
			'vbox': _gtk.gtk_dialog_get_vbox,
			'action_area': _gtk.gtk_dialog_get_action_area
		}
		if attrs.has_key(attr):
			return _obj2inst(attrs[attr](self._o))
		return GtkWindow.__getattr__(self, attr)

class GtkFileSelection(GtkWindow):
	get_type = _gtk.gtk_file_selection_get_type
	def __init__(self, title='', _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_file_selection_new(title)

	def __getattr__(self, attr):
		attrs = {
		'cancel_button':   _gtk.gtk_file_selection_get_cancel_button,
		'dir_list':        _gtk.gtk_file_selection_get_dir_list,
		'file_list':       _gtk.gtk_file_selection_get_file_list,
		'help_button':     _gtk.gtk_file_selection_get_help_button,
		'main_vbox':       _gtk.gtk_file_selection_get_main_vbox,
		'ok_button':       _gtk.gtk_file_selection_get_ok_button,
		'selection_entry': _gtk.gtk_file_selection_get_selection_entry,
		'selection_text':  _gtk.gtk_file_selection_get_selection_text
		}
		if attrs.has_key(attr):
			return _obj2inst(attrs[attr](self._o))
		return GtkWindow.__getattr__(self, attr)
	def set_filename(self, filename):
		_gtk.gtk_file_selection_set_filename(self._o, filename)
	def get_filename(self):
		return _gtk.gtk_file_selection_get_filename(self._o)
	def complete(self, pattern):
		_gtk.gtk_file_selection_complete(self._o, pattern)
	def show_fileop_buttons(self):
		_gtk.gtk_file_selection_show_fileop_buttons(self._o)
	def hide_fileop_buttons(self):
		_gtk.gtk_file_selection_hide_fileop_buttons(self._o)

class GtkInputDialog(GtkWindow):
	get_type = _gtk.gtk_input_dialog_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_input_dialog_new()
	def __getattr__(self, attr):
		attrs = {
			'close_button': _gtk.gtk_input_dialog_get_close_button,
			'save_button': _gtk.gtk_input_dialog_get_save_button
		}
		if attrs.has_key(attr):
			return _obj2inst(attrs[attr](self._o))
		return GtkWindow.__getattr__(self, attr)

class GtkFontSelectionDialog(GtkWindow):
	get_type = _gtk.gtk_font_selection_dialog_get_type
	def __init__(self, title=None, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_font_selection_dialog_new(title)
	def __getattr__(self, attr):
		attrs = {
		"fontsel": _gtk.gtk_font_selection_dialog_get_fontsel,
		"main_vbox": _gtk.gtk_font_selection_dialog_get_main_vbox,
		"action_area": _gtk.gtk_font_selection_dialog_get_action_area,
		"ok_button": _gtk.gtk_font_selection_dialog_get_ok_button,
		"apply_button":_gtk.gtk_font_selection_dialog_get_apply_button,
		"cancel_button":
		        _gtk.gtk_font_selection_dialog_get_cancel_button
		}
		if attrs.has_key(attr):
			return _obj2inst(attrs[attr](self._o))
		return GtkWindow.__getattr__(self, attr)
	def get_font_name(self):
		return _gtk.gtk_font_selection_dialog_get_font_name(self._o)
	def get_font(self):
		return _gtk.gtk_font_selection_dialog_get_font(self._o)
	def set_font_name(self, name):
		return _gtk.gtk_font_selection_dialog_set_font_name(self._o,
								    name)
	def get_preview_text(self):
		return _gtk.gtk_font_selection_dialog_get_preview_text(self._o)
	def set_preview_text(self, text):
		_gtk.gtk_font_selection_dialog_set_preview_text(self._o, text)

class GtkPlug(GtkWindow):
	get_type = _gtk.gtk_plug_get_type
	def __init__(self, socket_id=-1, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_plug_new(socket_id)

class GtkBox(GtkContainer):
	get_type = _gtk.gtk_box_get_type
	def __init__(self, _obj=None):
		if nonint: return
	def pack_start(self, child, expand=TRUE, fill=TRUE, padding=0):
		_gtk.gtk_box_pack_start(self._o, child._o, expand,fill,padding)
	def pack_end(self, child, expand=TRUE, fill=TRUE, padding=0):
		_gtk.gtk_box_pack_end(self._o, child._o, expand,fill,padding)
	def reorder_child(self, child, pos):
		_gtk.gtk_box_reorder_child(self._o, child._o, pos)
	def set_homogeneous(self, homogeneous):
		_gtk.gtk_box_set_homogeneous(self._o, homogeneous)
	def set_spacing(self, spacing):
		_gtk.gtk_box_set_spacing(self._o, spacing)
	def query_child_packing(self, child):
		return _gtk.gtk_box_query_child_packing(self._o, child._o)
	def set_child_packing(self, child, expand, fill, padding, pack_type):
		_gtk.gtk_box_set_child_packing(self._o, child._o, expand, fill,
					       padding, pack_type)

class GtkHBox(GtkBox):
	get_type = _gtk.gtk_hbox_get_type
	def __init__(self, homogeneous=0, spacing=0, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_hbox_new(homogeneous, spacing)

class GtkCombo(GtkHBox):
	get_type = _gtk.gtk_combo_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_combo_new()
	def disable_activate(self, obj=None):
		_gtk.gtk_combo_disable_activate(self._o)
	def set_use_arrows(self, val):
		_gtk.gtk_combo_set_use_arrows(self._o, val)
	def set_use_arrows_always(self, val):
		_gtk.gtk_combo_set_use_arrows_always(self._o, val)
	def set_case_sensitive(self, val):
		_gtk.gtk_combo_set_case_sensitive(self._o, val)
	def set_item_string(self, item, val):
		_gtk.gtk_combo_set_item_string(self._o, item._o, val)
	def set_value_in_list(self, val, ok_if_empty):
		_gtk.gtk_combo_set_value_in_list(self._o, val, ok_if_empty)
	def set_popdown_strings(self, strings):
		_gtk.gtk_combo_set_popdown_strings(self._o, strings)
	def __getattr__(self, attr):
		attrs = {
			'entry': _gtk.gtk_combo_get_entry,
			'list':  _gtk.gtk_combo_get_list
	        }
		if attrs.has_key(attr):
			return _obj2inst(attrs[attr](self._o))
		return GtkHBox.__getattr__(self, attr)

class GtkStatusbar(GtkHBox):
	get_type = _gtk.gtk_statusbar_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_statusbar_new()
	def get_context_id(self, desc):
		return _gtk.gtk_statusbar_get_context_id(self._o, desc)
	def pop(self, cid):
		_gtk.gtk_statusbar_pop(self._o, cid)
	def push(self, cid, text):
		return _gtk.gtk_statusbar_push(self._o, cid, text)
	def remove(self, cid, msg_id):
		_gtk.gtk_statusbar_remove(self._o, cid, msg_id)

class GtkVBox(GtkBox):
	get_type = _gtk.gtk_vbox_get_type
	def __init__(self, homogeneous=0, spacing=0, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_vbox_new(homogeneous, spacing)

class GtkColorSelection(GtkVBox):
	get_type = _gtk.gtk_color_selection_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_color_selection_new()
	def set_update_policy(self, p):
		_gtk.gtk_color_selection_set_update_policy(self._o, p)
	def set_opacity(self, use_opacity):
		_gtk.gtk_color_selection_set_opacity(self._o, use_opacity)
	def set_color(self, c):
		_gtk.gtk_color_selection_set_color(self._o, c)
	def get_color(self):
		return _gtk.gtk_color_selection_get_color(self._o)

class GtkGammaCurve(GtkVBox):
	get_type = _gtk.gtk_gamma_curve_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_gamma_curve_new()
	def __getattr__(self, attr):
		if attr == 'gamma':
			return _gtk.gtk_gamma_curve_get_gamma(self._o)
		attrs = {
			'curve': _gtk.gtk_gamma_curve_get_curve,
			'gamma_dialog': _gtk.gtk_gamma_curve_get_gamma_dialog,
			'gamma_text': _gtk.gtk_gamma_curve_get_gamma_text,
			'table': _gtk.gtk_gamma_curve_get_table,
		}
		if attrs.has_key(attr):
			return _obj2inst(attrs[attr](self._o))
		return GtkVBox.__getattr__(self, attr)

class GtkButtonBox(GtkBox):
	get_type = _gtk.gtk_button_box_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def get_child_size_default(self):
		return _gtk.gtk_button_box_get_child_size_default()
	def get_child_ipadding_default(self):
		return _gtk.gtk_button_box_get_child_ipadding_default()
	def set_child_size_default(self, mw, mh):
		_gtk.gtk_button_box_set_child_size_default(mw, mh)
	def set_child_ipadding_default(self, ix, iy):
		_gtk.gtk_button_box_set_child_ipadding_default(ix, iy)
	def get_spacing(self):
		return _gtk.gtk_button_box_get_spacing(self._o)
	def get_layout(self):
		return _gtk.gtk_button_box_get_layout(self._o)
	def get_child_size(self):
		return _gtk.gtk_button_box_get_child_size(self._o)
	def get_child_ipadding(self):
		return _gtk.gtk_button_box_get_child_ipadding(self._o)
	def set_spacing(self, spacing):
		_gtk.gtk_button_box_set_spacing(self._o, spacing)
	def set_layout(self, layout):
		_gtk.gtk_button_box_set_layout(self._o, layout)
	def set_child_size(self, mw, mh):
		_gtk.gtk_button_box_set_child_size(self._o, mw, mh)
	def set_child_ipadding(self, ix, iy):
		_gtk.gtk_button_box_set_child_ipadding(self._o, ix, iy)

class GtkHButtonBox(GtkButtonBox):
	get_type = _gtk.gtk_hbutton_box_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_hbutton_box_new()
	def get_spacing_default(self):
		return _gtk.gtk_hbutton_box_get_spacing_default()
	def get_layout_default(self):
		return _gtk.gtk_hbutton_box_get_layout_default()
	def set_spacing_default(self, spacing):
		_gtk.gtk_hbutton_box_set_spacing_default(spacing)
	def set_layout_default(self, layout):
		_gtk.gtk_hbutton_box_set_layout_default(layout)

class GtkVButtonBox(GtkButtonBox):
	get_type = _gtk.gtk_vbutton_box_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_vbutton_box_new()
	def get_spacing_default(self):
		return _gtk.gtk_vbutton_box_get_spacing_default()
	def get_layout_default(self):
		return _gtk.gtk_vbutton_box_get_layout_default()
	def set_spacing_default(self, spacing):
		_gtk.gtk_vbutton_box_set_spacing_default(spacing)
	def set_layout_default(self, layout):
		_gtk.gtk_vbutton_box_set_layout_default(layout)

class GtkCList(GtkContainer):
	get_type = _gtk.gtk_clist_get_type
	def __init__(self, cols=1, titles=None, _obj=None):
		if _obj: self._o = _obj; return
		if titles == None:
			self._o = _gtk.gtk_clist_new(cols)
		else:
			self._o = _gtk.gtk_clist_new_with_titles(cols, titles)
	def __getattr__(self, attr):
		attrs = {
			'selection':_gtk.gtk_clist_get_selection,
			'focus_row':_gtk.gtk_clist_get_focus_row,
			'columns': _gtk.gtk_clist_get_columns,
			'rows':_gtk.gtk_clist_get_rows,
			'row_height':_gtk.gtk_clist_get_row_height,
			'sort_column':_gtk.gtk_clist_get_sort_column,
			'sort_type':_gtk.gtk_clist_get_sort_type,
		}
		if attrs.has_key(attr):
			return attrs[attr](self._o)
		return GtkContainer.__getattr__(self, attr)
	def append(self, values):
		return _gtk.gtk_clist_append(self._o, values)
	def clear(self, obj=None):
		_gtk.gtk_clist_clear(self._o)
	def column_title_active(self, col):
		_gtk.gtk_clist_column_title_active(self._o, col)
	def column_title_passive(self, col):
		_gtk.gtk_clist_column_title_passive(self._o, col)
	def column_titles_active(self, obj=None):
		_gtk.gtk_clist_column_titles_active(self._o)
	def column_titles_hide(self, obj=None):
		_gtk.gtk_clist_column_titles_hide(self._o)
	def column_titles_passive(self, obj=None):
		_gtk.gtk_clist_column_titles_passive(self._o)
	def column_titles_show(self, obj=None):
		_gtk.gtk_clist_column_titles_show(self._o)
	def columns_autosize(self):
		return _gtk.gtk_clist_columns_autosize(self._o)
	def find_row_from_data(self, data):
		return _gtk.gtk_clist_find_row_from_data(self._o, data)
	def freeze(self, obj=None):
		_gtk.gtk_clist_freeze(self._o)
	def get_cell_style(self, row, col):
		return _gtk.gtk_clist_get_cell_style(self._o, row, col)
	def get_cell_type(self, r, c):
		return _gtk.gtk_clist_get_cell_type(self._o, r, c)
	def get_column_title(self, col):
		return _gtk.gtk_clist_get_column_title(self._o, col)
	def get_column_widget(self, col):
		return _obj2inst(_gtk.gtk_clist_get_column_widget(self._o,col))
	def get_column_width(self, col):
		return _gtk.gtk_clist_get_column_width(self._o, col)
	def get_row_data(self, row):
		return _gtk.gtk_clist_get_row_data(self._o, row)
	def get_row_style(self, row):
		return _gtk.gtk_clist_get_row_style(self._o, row)
	def get_text(self, r, c):
		return _gtk.gtk_clist_get_text(self._o, r, c)
	def get_pixmap(self, r, c):
		return _gtk.gtk_clist_get_pixmap(self._o, r, c)
	def get_pixtext(self, r, c):
		return _gtk.gtk_clist_get_pixtext(self._o, r, c)
	def get_selectable(self, row):
		return _gtk.gtk_clist_get_selectable(self._o, row)
	def get_selection_info(self, x, y):
		return _gtk.gtk_clist_get_selection_info(self._o, x, y)
	def insert(self, row, values):
		return _gtk.gtk_clist_insert(self._o, row, values)
	def moveto(self, row=-1, col=-1, row_align=0.5, col_align=0.5):
		_gtk.gtk_clist_moveto(self._o, row, col, row_align, col_align)
	def optimal_column_width(self, col):
		return _gtk.gtk_clist_optimal_column_width(self._o, col)
	def remove(self, row):
		_gtk.gtk_clist_remove(self._o, row)
	def row_is_visible(self, row):
		return _gtk.gtk_clist_row_is_visible(self._o, row)
	def row_move(self, source_row, dest_row):
		_gtk.gtk_clist_row_move(self._o, source_row, dest_row)
	def select_all(self):
		_gtk.gtk_clist_select_all(self._o)
	def select_row(self, row, col):
		_gtk.gtk_clist_select_row(self._o, row, col)
	def set_auto_sort(self, auto_sort):
		_gtk.gtk_clist_set_auto_sort(self._o, auto_sort)
	def set_background(self, row, colour):
		_gtk.gtk_clist_set_background(self._o, row, colour)
	def set_cell_style(self, row, col, style):
		_gtk.gtk_clist_set_cell_style(self._o, row, col, style)
	def set_column_auto_resize(self, col, auto):
		_gtk.gtk_clist_set_column_auto_resize(self._o, col, auto)
	def set_column_justification(self, col, just):
		_gtk.gtk_clist_set_column_justification(self._o, col, just)
	def set_column_max_width(self, col, width):
		_gtk.gtk_clist_set_column_max_width(self._o, col, width)
	def set_column_min_width(self, col, width):
		_gtk.gtk_clist_set_column_min_width(self._o, col, width)
	def set_column_resizeable(self, col, resizeable):
		_gtk.gtk_clist_set_column_resizeable(self._o, col, resizeable)
	def set_column_title(self, col, title):
		_gtk.gtk_clist_set_column_title(self._o, col, title)
	def set_column_visibility(self, col, visible):
		_gtk.gtk_clist_set_column_visibility(self._o, col, visible)
	def set_column_widget(self, col, w):
		_gtk.gtk_clist_set_column_widget(self._o, col, w._o)
	def set_column_width(self, col, width):
		_gtk.gtk_clist_set_column_width(self._o, col, width)
	def set_foreground(self, row, colour):
		_gtk.gtk_clist_set_foreground(self._o, row, colour)
	def set_hadjustment(self, adj):
		_gtk.gtk_clist_set_hadjustment(self._o, adj._o)
	def set_vadjustment(self, adj):
		_gtk.gtk_clist_set_vadjustment(self._o, adj._o)
	def get_hadjustment(self):
		return GtkAdjustment(_obj=
				    _gtk.gtk_clist_get_hadjustment(self._o))
	def get_vadjustment(self):
		return GtkAdjustment(_obj=
				    _gtk.gtk_clist_get_vadjustment(self._o))
	def set_reorderable(self, reorderable):
		_gtk.gtk_clist_set_reorderable(self._o, reorderable)
	def set_use_drag_icons(self, use_icons):
		_gtk.gtk_clist_set_use_drag_icons(self._o, use_icons)
	def set_button_actions(self, button, button_actions):
		_gtk.gtk_clist_set_button_actions(self._o, button,
						  button_actions)
	def set_row_data(self, row, data):
		_gtk.gtk_clist_set_row_data(self._o, row, data)
	def set_row_height(self, height):
		_gtk.gtk_clist_set_row_height(self._o, height)
	def set_row_style(self, row, style):
		_gtk.gtk_clist_set_row_style(self._o, row, style)
	def set_shift(self, row, col, v, h):
		_gtk.gtk_clist_set_shift(self._o, row, col, v, h)
	def set_selectable(self, row, selectable):
		_gtk.gtk_clist_set_selectable(self._o, row, selectable)
	def set_selection_mode(self, mode):
		_gtk.gtk_clist_set_selection_mode(self._o, mode)
	def set_shadow_type(self, border):
		_gtk.gtk_clist_set_shadow_type(self._o, border)
	def set_sort_column(self, col):
		_gtk.gtk_clist_set_sort_column(self._o, col)
	def set_sort_type(self, sort_type):
		_gtk.gtk_clist_set_sort_type(self._o, sort_type)
	def set_text(self, r, c, text):
		_gtk.gtk_clist_set_text(self._o, r, c, text)
	def set_pixmap(self, r, c, pixmap, mask=None):
		if not mask and hasattr(pixmap, '_o'):
			pixmap, mask = pixmap.get()
		_gtk.gtk_clist_set_pixmap(self._o, r, c, pixmap, mask)
	def set_pixtext(self, r, c, text, spacing, pixmap, mask=None):
		if not mask and hasattr(pixmap, '_o'):
			pixmap, mask = pixmap.get()
		_gtk.gtk_clist_set_pixtext(self._o, r, c, text, spacing,
					   pixmap, mask)
	def sort(self):
		_gtk.gtk_clist_sort(self._o)
	def swap_rows(self, row1, row2):
		_gtk.gtk_clist_swap_rows(self._o, row1, row2)
	def thaw(self):
		_gtk.gtk_clist_thaw(self._o)
	def undo_selection(self):
		_gtk.gtk_clist_undo_selection(self._o)
	def unselect_all(self):
		_gtk.gtk_clist_unselect_all(self._o)
	def unselect_row(self, row, col):
		_gtk.gtk_clist_unselect_row(self._o, row, col)

class GtkCTree(GtkCList):
	get_type = _gtk.gtk_ctree_get_type
	def __init__(self, cols=1, tree_col=0, titles=None, _obj=None):
		if _obj: self._o = _obj; return
		if titles == None:
			self._o = _gtk.gtk_ctree_new(cols, tree_col)
		else:
			self._o = _gtk.gtk_ctree_new_with_titles(cols,
								 tree_col,
								 titles)
	def __getattr__(self, attr):
		attrs = {
			'selection':_gtk.gtk_ctree_get_selection,
		}
		if attrs.has_key(attr):
			return attrs[attr](self._o)
		return GtkCList.__getattr__(self, attr)
	def base_nodes(self):
		# this returns a list of the base nodes.  Useful for recursion
		return _gtk.gtk_ctree_base_nodes(self._o)
	def insert_node(self, parent, sibling, text, spacing=5,
			pixmap_closed=None, mask_closed=None,
			pixmap_opened=None, mask_opened=None, is_leaf=TRUE,
			expanded=FALSE):
		# parent and sibling can also be None
		return _gtk.gtk_ctree_insert_node(self._o, parent, sibling,
						  text, spacing,
						  pixmap_closed, mask_closed,
						  pixmap_opened, mask_opened,
						  is_leaf, expanded)
	def remove_node(self, node):
		_gtk.gtk_ctree_remove_node(self._o, node)
	def is_viewable(self, node):
		return _gtk.gtk_ctree_is_viewable(self._o, node)
	def last(self, node):
		return _gtk.gtk_ctree_last(self._o, node)
	def node_nth(self, row):
		return _gtk.gtk_ctree_node_nth(self._o, row)
	def find(self, node, child):
		return _gtk.gtk_ctree_find(self._o, node, child)
	def is_ancestor(self, node, child):
		return _gtk.gtk_ctree_is_ancestor(self._o, node, child)
	def find_by_row_data(self, node, data):
		return _gtk.gtk_ctree_find_by_row_data(self._o, node, data)
	def find_all_by_row_data(self, node, data):
		return _gtk.gtk_ctree_find_all_by_row_data(self._o, node, data)
	def is_hot_spot(self, x, y):
		return _gtk.gtk_ctree_is_hot_spot(self._o, x, y)
	def move(self, node, new_parent, new_sibling):
		_gtk.gtk_ctree_move(self._o, node, new_parent, new_sibling)
	def expand(self, node):
		_gtk.gtk_ctree_expand(self._o, node)
	def expand_recursive(self, node):
		_gtk.gtk_ctree_expand_recursive(self._o, node)
	def expand_to_depth(self, node, depth):
		_gtk.gtk_ctree_expand_to_depth(self._o, node, depth)
	def collapse(self, node):
		_gtk.gtk_ctree_collapse(self._o, node)
	def collapse_recursive(self, node):
		_gtk.gtk_ctree_collapse_recursive(self._o, node)
	def collapse_to_depth(self, node, depth):
		_gtk.gtk_ctree_collapse_to_depth(self._o, node, depth)
	def toggle_expansion(self, node):
		_gtk.gtk_ctree_toggle_expansion(self._o, node)
	def toggle_expansion_recursive(self, node):
		_gtk.gtk_ctree_toggle_expansion_recursive(self._o, node)
	def select(self, node):
		_gtk.gtk_ctree_select(self._o, node)
	def select_recursive(self, node):
		_gtk.gtk_ctree_select_recursive(self._o, node)
	def unselect(self, node):
		_gtk.gtk_ctree_unselect(self._o, node)
	def unselect_recursive(self, node):
		_gtk.gtk_ctree_unselect_recursive(self._o, node)
	def real_select_recursive(self, node, state):
		_gtk.gtk_ctree_real_select_recursive(self._o, node, state)
	def node_set_text(self, node, col, text):
		_gtk.gtk_ctree_node_set_text(self._o, node, col, text)
	def node_set_pixmap(self, node, col, pixmap, mask):
		_gtk.gtk_ctree_node_set_pixmap(self._o, node, col, pixmap,mask)
	def node_set_pixtext(self, node, col, text, spacing, pixmap, mask):
		_gtk.gtk_ctree_node_set_pixtext(self._o, node, col, text,
						spacing, pixmap, mask)
	def set_node_info(self, node, text, spacing, pixmap_closed,
			  mask_closed, pixmap_opened, mask_opened, is_leaf,
			  expanded):
		_gtk.gtk_ctree_set_node_info(self._o, node, text, spacing,
					     pixmap_closed, mask_closed,
					     pixmap_opened, mask_opened,
					     is_leaf, expanded)
	def node_set_shift(self, node, col, vert, horiz):
		_gtk.gtk_ctree_node_set_shift(self._o, node, col, vert, horiz)
	def node_get_selectable(self, node):
		return _gtk.gtk_ctree_node_get_selectable(self._o, node)
	def node_set_selectable(self, node, selectable):
		_gtk.gtk_ctree_node_set_selectable(self._o, node, selectable)
	def node_get_cell_type(self, node, col):
		return _gtk.gtk_ctree_node_get_cell_type(self._o, node, col)
	def node_get_text(self, node, col):
		return _gtk.gtk_ctree_node_get_text(self._o, node, col)
	def node_get_pixmap(self, node, col):
		return _gtk.gtk_ctree_node_get_pixmap(self._o, node, col)
	def node_get_pixtext(self, node, col):
		return _gtk.gtk_ctree_node_get_pixtext(self._o, node, col)
	def get_node_info(self, node):
		return _gtk.gtk_ctree_get_node_info(self._o, node)
	def node_set_row_style(self, node, style):
		_gtk.gtk_ctree_node_set_row_style(self._o, node, style)
	def node_get_row_style(self, node):
		return _gtk.gtk_ctree_node_get_row_style(self._o, node)
	def node_set_cell_style(self, node, col, style):
		_gtk.gtk_ctree_node_set_cell_style(self._o, node, col, style)
	def node_get_cell_style(self, node, col):
		return _gtk.gtk_ctree_node_get_cell_style(self._o, node, col)
	def node_set_foreground(self, node, color):
		_gtk.gtk_ctree_node_set_foreground(self._o, node, color)
	def node_set_background(self, node, color):
		_gtk.gtk_ctree_node_set_background(self._o, node, color)
	def node_set_row_data(self, node, data):
		_gtk.gtk_ctree_node_set_row_data(self._o, node, data)
	def node_get_row_data(self, node):
		return _gtk.gtk_ctree_node_get_row_data(self._o, node)
	def node_moveto(self, node, col, row_align, col_align):
		_gtk.gtk_ctree_node_moveto(self._o, node, col, row_align,
					   col_align)
	def node_is_visible(self, node):
		return _gtk.gtk_ctree_node_is_visible(self._o, node)
	def set_indent(self, indent):
		_gtk.gtk_ctree_set_indent(self._o, indent)
	def set_spacing(self, spacing):
		_gtk.gtk_ctree_set_spacing(self._o, spacing)
	def set_show_stub(self, show_stub):
		_gtk.gtk_ctree_set_show_stub(self._o, show_stub)
	def set_line_style(self, style):
		_gtk.gtk_ctree_set_line_style(self._o, style)
	def set_expander_style(self, style):
		_gtk.gtk_ctree_set_expander_style(self._o, style)
	def sort_node(self, node):
		_gtk.gtk_ctree_sort_node(self._o, node)
	def sort_recursive(self, node):
		_gtk.gtk_ctree_sort_recursive(self._o, node)

class GtkFixed(GtkContainer):
	get_type = _gtk.gtk_fixed_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_fixed_new()
	def put(self, child, x, y):
		_gtk.gtk_fixed_put(self._o, child._o, x, y)
	def move(self, child, x, y):
		_gtk.gtk_fixed_move(self._o, child._o, x, y)

class GtkLayout(GtkContainer):
	get_type = _gtk.gtk_layout_get_type
	def __init__(self, hadj=None, vadj=None, _obj=None):
		if _obj: self._o = _obj; return
		if hadj and vadj:
			self._o = _gtk.gtk_layout_new(hadj._o, vadj._o)
		else:
			self._o = _gtk.gtk_layout_new()
	def put(self, child, x, y):
		_gtk.gtk_layout_put(self._o, child._o, x, y)
	def move(self, child, x, y):
		_gtk.gtk_layout_move(self._o, child._o, x, y)
	def set_size(self, width, height):
		_gtk.gtk_layout_set_size(self._o, width, height)
	def freeze(self):
		_gtk.gtk_layout_freeze(self._o)
	def thaw(self):
		_gtk.gtk_layout_thaw(self._o)
	def get_hadjustment(self):
		return GtkAdjustment(
			_obj=_gtk.gtk_layout_get_hadjustment(self._o))
	def get_vadjustment(self):
		return GtkAdjustment(
			_obj=_gtk.gtk_layout_get_vadjustment(self._o))
	def set_hadjustment(self, adj):
		_gtk.gtk_layout_set_hadjustment(self._o, adj._o)
	def set_vadjustment(self, adj):
		_gtk.gtk_layout_set_vadjustment(self._o, adj._o)

class GtkList(GtkContainer):
	get_type = _gtk.gtk_list_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_list_new()
	def __getattr__(self, attr):
		attrs = {
			'last_focus_child':_gtk.gtk_list_get_last_focus_child,
			'undo_focus_child':_gtk.gtk_list_get_undo_focus_child,
		}
		if attrs.has_key(attr):
			return _obj2inst(attrs[attr](self._o))
		return GtkContainer.__getattr__(self, attr)
	def get_selection(self):
		l = _gtk.gtk_list_get_selection(self._o)
		return map(_obj2inst, l)
	def insert_items(self, items, pos):
		items = map(lambda i: i._o, items)
		_gtk.gtk_list_insert_items(self._o, items, pos)
	def append_items(self, items):
		items = map(lambda i: i._o, items)
		_gtk.gtk_list_append_items(self._o, items)
	def prepend_items(self, items):
		items = map(lambda i: i._o, items)
		_gtk.gtk_list_prepend_items(self._o, items)
	def remove_items(self, items):
		items = map(lambda i: i._o, items)
		_gtk.gtk_list_remove_items(self._o, items)
	def clear_items(self, start, end):
		_gtk.gtk_list_clear_items(self._o, start, end)
	def select_item(self, item):
		_gtk.gtk_list_select_item(self._o, item)
	def unselect_item(self, item):
		_gtk.gtk_list_unselect_item(self._o, item)
	def select_child(self, child):
		_gtk.gtk_list_select_child(self._o, child._o)
	def unselect_child(self, child):
		_gtk.gtk_list_unselect_child(self._o, child._o)
	def child_position(self, child):
		return _gtk.gtk_list_child_position(self._o, child._o)
	def set_selection_mode(self, mode):
		_gtk.gtk_list_set_selection_mode(self._o, mode)
	def extend_selection(self, scroll_type, pos, auto):
		_gtk.gtk_list_extend_selection(self._o, scroll_type, pos, auto)
	def start_selection(self):
		_gtk.gtk_list_start_selection(self._o)
	def end_selection(self):
		_gtk.gtk_list_end_selection(self._o)
	def select_all(self):
		_gtk.gtk_list_select_all(self._o)
	def unselect_all(self):
		_gtk.gtk_list_unselect_all(self._o)
	def scroll_horizontal(self, scroll_type, pos):
		_gtk.gtk_list_scroll_horizontal(self._o, scroll_type, pos)
	def scroll_vertical(self, scroll_type, pos):
		_gtk.gtk_list_scroll_vertical(self._o, scroll_type, pos)
	def toggle_add_mode(self):
		_gtk.gtk_list_toggle_add_mode(self._o)
	def toggle_focus_row(self):
		_gtk.gtk_list_toggle_focus_row(self._o)
	def toggle_row(self, item):
		_gtk.gtk_list_toggle_row(self._o, item._o)
	def undo_selection(self):
		_gtk.gtk_list_undo_selection(self._o)
	def end_drag_selection(self):
		_gtk.gtk_list_end_drag_selection(self._o)

class GtkMenuShell(GtkContainer):
	get_type = _gtk.gtk_menu_shell_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def append(self, child):
		_gtk.gtk_menu_shell_append(self._o, child._o)
	def prepend(self, child):
		_gtk.gtk_menu_shell_prepend(self._o, child._o)
	def insert(self, child, pos):
		_gtk.gtk_menu_shell_insert(self._o, child._o, pos)
	def deactivate(self, obj=None):
		_gtk.gtk_menu_shell_deactivate(self._o)
	def select_item(self, item):
		_gtk.gtk_menu_shell_select_item(self._o, item._o)
	def deselect(self):
		_gtk.gtk_menu_shell_deselect(self._o)
	def activate_item(self, item, force_deactivate):
		_gtk.gtk_menu_shell_activate_item(self._o, item._o,
						  force_deactivate)

class GtkMenuBar(GtkMenuShell):
	get_type = _gtk.gtk_menu_bar_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_menu_bar_new()
	def append(self, child):
		_gtk.gtk_menu_bar_append(self._o, child._o)
	def prepend(self, child):
		_gtk.gtk_menu_bar_prepend(self._o, child._o)
	def insert(self, child, pos):
		_gtk.gtk_menu_bar_insert(self._o, child._o, pos)
	def set_shadow_type(self, type):
		_gtk.gtk_menu_bar_set_shadow_type(self._o, type)

class GtkMenu(GtkMenuShell):
	get_type = _gtk.gtk_menu_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_menu_new()
	def append(self, child):
		_gtk.gtk_menu_append(self._o, child._o)
	def prepend(self, child):
		_gtk.gtk_menu_prepend(self._o, child._o)
	def insert(self, child, pos):
		_gtk.gtk_menu_insert(self._o, child._o, pos)
	def popup(self, pms, pmi, func, button, time):
		if pms: pms = pms._o
		if pmi: pmi = pmi._o
		_gtk.gtk_menu_popup(self._o, pms, pmi, func, button, time)
	def reposition(self):
		_gtk.gtk_menu_reposition(self._o)
	def popdown(self, obj=None):
		_gtk.gtk_menu_popdown(self._o)
	def get_active(self):
		return _obj2inst(_gtk.gtk_menu_get_active(self._o))
	def get_attach_widget(self):
		return _obj2inst(_gtk.gtk_menu_get_attach_widget(self._o))
	def detach(self):
		_gtk.gtk_menu_detach(self._o)
	def set_active(self, index):
		_gtk.gtk_menu_set_active(self._o, index)
	def set_accel_group(self, group):
		_gtk.gtk_menu_set_accel_group(self._o, group._ag)
	def get_accel_group(self):
		return GtkAccelGroup(_obj=
				     _gtk.gtk_menu_get_accel_group(self._o))
	def set_tearoff_state(self, torn_off):
		_gtk.gtk_menu_set_tearoff_state(self._o, torn_off)
	def set_title(self, title):
		_gtk.gtk_menu_set_title(self._o, title)
	def reorder_child(self, child, position):
		_gtk.gtk_menu_reorder_child(self._o, child._o, position)
	def ensure_uline_accel_group(self):
		return GtkAccelGroup(
			_obj=_gtk.gtk_menu_ensure_uline_accel_group(self._o))

class GtkNotebook(GtkContainer):
	get_type = _gtk.gtk_notebook_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_notebook_new()
	def append_page(self, child, tab):
		if tab: tab = tab._o
		_gtk.gtk_notebook_append_page(self._o, child._o, tab)
	def append_page_menu(self, child, tab, ml):
		if tab: tab = tab._o
		if ml: ml = ml._o
		_gtk.gtk_notebook_append_page_menu(self._o, child._o, tab, ml)
	def prepend_page(self, child, tab):
		if tab: tab = tab._o
		_gtk.gtk_notebook_prepend_page(self._o, child._o, tab)
	def prepend_page_menu(self, child, tab, ml):
		if tab: tab = tab._o
		if ml: ml = ml._o
		_gtk.gtk_notebook_prepend_page_menu(self._o, child._o, tab, ml)
	def insert_page(self, child, tab, pos):
		if tab: tab = tab._o
		_gtk.gtk_notebook_insert_page(self._o, child._o, tab, pos)
	def insert_page_menu(self, child, tab, ml, pos):
		if tab: tab = tab._o
		if ml: ml = ml._o
		_gtk.gtk_notebook_insert_page_menu(self._o, child._o, tab,
						   ml, pos)
	def remove_page(self, pos):
		_gtk.gtk_notebook_remove_page(self._o, pos)
	def current_page(self):
		print "current_page deprecated -- use get_current_page"
		return self.current_page()
	def get_current_page(self):
		return _gtk.gtk_notebook_get_current_page(self._o)
	def get_nth_page(self, page_num):
		return _obj2inst(_gtk.gtk_notebook_get_nth_page(self._o,
								page_num))
	def page_num(self, child):
		return _gtk.gtk_notebook_page_num(self._o, child._o)
	def set_page(self, pos):
		_gtk.gtk_notebook_set_page(self._o, pos)
	def next_page(self, obj=None):
		_gtk.gtk_notebook_next_page(self._o)
	def prev_page(self, obj=None):
		_gtk.gtk_notebook_prev_page(self._o)
	def reorder_child(self, child, pos):
		_gtk.gtk_notebook_reorder_child(self._o, child._o, pos)
	def set_tab_border(self, border):
		_gtk.gtk_notebook_set_tab_border(self._o, border)
	def set_tab_hborder(self, tab_hborder):
		_gtk.gtk_notebook_set_tab_hborder(self._o, tab_hborder)
	def set_tab_vborder(self, tab_vborder):
		_gtk.gtk_notebook_set_tab_vborder(self._o, tab_vborder)
	def get_tab_pos(self):
		return _gtk.gtk_notebook_get_tab_pos(self._o)
	def set_tab_pos(self, pos):
		_gtk.gtk_notebook_set_tab_pos(self._o, pos)
	def set_homogeneous_tabs(self, homog):
		_gtk.gtk_notebook_set_homogeneous_tabs(self._o, homog)
	def set_scrollable(self, scrollable):
		_gtk.gtk_notebook_set_scrollable(self._o, scrollable)
	def set_show_tabs(self, show):
		_gtk.gtk_notebook_set_show_tabs(self._o, show)
	def set_show_border(self, show):
		_gtk.gtk_notebook_set_show_border(self._o, show)
	def popup_enable(self, obj=None):
		_gtk.gtk_notebook_popup_enable(self._o)
	def popup_disable(self, obj=None):
		_gtk.gtk_notebook_popup_disable(self._o)
	def query_tab_label(self, child):
		print "query_tab_label deprecated -- use get_tab_label"
		return self.get_tab_label(child)
	def get_tab_label(self, child):
		return _obj2inst(_gtk.gtk_notebook_get_tab_label(self._o,
								 child._o))
	def set_tab_label(self, child, tab_label):
		_gtk.gtk_notebook_set_tab_label(self._o, child._o,
						tab_label._o)
	def set_tab_label_text(self, child, tab_text):
		_gtk.gtk_notebook_set_tab_label_text(self._o, child._o,
						     tab_text)
	def query_menu_label(self, child):
		print "query_menu_label deprecated -- use get_menu_label"
		return self.get_menu_label(child)
	def get_menu_label(self, child):
		return _obj2inst(_gtk.gtk_notebook_get_menu_label(self._o,
								  child._o))
	def set_menu_label(self, child, menu_label):
		_gtk.gtk_notebook_set_menu_label(self._o, child._o,
						menu_label._o)
	def set_menu_label_text(self, child, menu_text):
		_gtk.gtk_notebook_set_menu_label_text(self._o, child._o,
						      menu_text)
	def query_tab_label_packing(self, child):
		# returns (expand,fill,pack_type)
		return _gtk.gtk_notebook_query_tab_label_packing(self._o,
								 child._o)
	def set_tab_label_packing(self, child, expand, fill, pack_type):
		_gtk.gtk_notebook_set_tab_label_packing(self._o, child._o,
							expand, fill,
							pack_type)

class GtkFontSelection(GtkNotebook):
	get_type = _gtk.gtk_font_selection_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_font_selection_new()
	def get_font_name(self):
		return _gtk.gtk_font_selection_get_font_name(self._o)
	def get_font(self):
		return _gtk.gtk_font_selection_get_font(self._o)
	def set_font_name(self, name):
		return _gtk.gtk_font_selection_set_font_name(self._o, name)
	def get_preview_text(self):
		return _gtk.gtk_font_selection_get_preview_text(self._o)
	def set_preview_text(self, text):
		_gtk.gtk_font_selection_set_preview_text(self._o, text)

class GtkPacker(GtkContainer):
	get_type = _gtk.gtk_packer_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_packer_new()
	def add_defaults(self, child, side, anchor, options):
		_gtk.gtk_packer_add_defaults(self._o, child._o, side, anchor,
					     options)
	def add(self, child, side=SIDE_TOP, anchor=ANCHOR_CENTER,
		options=0, border_width=0, pad_x=0, pad_y=0,
		 i_pad_x=0, i_pad_y=0):
		_gtk.gtk_packer_add(self._o, child._o, side, anchor, options,
				    border_width, pad_x, pad_y, i_pad_x,
				    i_pad_y)
	def configure(self, child, side, anchor, options, border_width, pad_x,
		      pad_y, i_pad_x, i_pad_y):
		print "configure deprecated -- use set_child_packing"
		self.set_child_packing(child, side, anchor, options,
				       border_width, pad_x, pad_y, i_pad_x,
				       i_pad_y)
	def set_child_packing(self, child, side, anchor, options,
			      border_width, pad_x, pad_y, i_pad_x, i_pad_y):
		_gtk.gtk_packer_set_child_packing(self._o, child._o, side,
						  anchor, options,
						  border_width, pad_x, pad_y,
						  i_pad_x, i_pad_y)
	def reorder_child(self, child, pos):
		_gtk.gtk_packer_reorder_child(self._o, child._o, pos)
	def set_spacing(self, spacing):
		_gtk.gtk_packer_set_spacing(self._o, spacing)
	def set_default_pad(self, pad_x, pad_y):
		_gtk.gtk_packer_set_default_pad(self._o, pad_x, pad_y)
	def set_default_ipad(self, i_pad_x, i_pad_y):
		_gtk.gtk_packer_set_default_ipad(self._o, i_pad_x, i_pad_y)

class GtkPaned(GtkContainer):
	get_type = _gtk.gtk_paned_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def __getattr__(self, attr):
		attrs = {
			'handle_size': _gtk.gtk_paned_get_handle_size,
			'gutter_size': _gtk.gtk_paned_get_gutter_size,
		}
		return attrs[attr](self._o)
	def add1(self, child):
		_gtk.gtk_paned_add1(self._o, child._o)
	def add2(self, child):
		_gtk.gtk_paned_add2(self._o, child._o)
	def pack1(self, child, resize=TRUE, shrink=TRUE):
		_gtk.gtk_paned_pack1(self._o, child._o, resize, shrink)
	def pack2(self, child, resize=TRUE, shrink=TRUE):
		_gtk.gtk_paned_pack2(self._o, child._o, resize, shrink)
	def set_position(self, position):
		_gtk.gtk_paned_set_position(self._o, position)
	def handle_size(self, size):
		print "handle_size deprecated -- use set_handle_size"
		self.set_handle_size(size)
	def set_handle_size(self, size):
		_gtk.gtk_paned_set_handle_size(self._o, size)
	def gutter_size(self, size):
		print "gutter_size deprecated -- use set_gutter_size"
		self.set_gutter_size(size)
	def set_gutter_size(self, size):
		_gtk.gtk_paned_set_gutter_size(self._o, size)

class GtkHPaned(GtkPaned):
	get_type = _gtk.gtk_hpaned_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_hpaned_new()

class GtkVPaned(GtkPaned):
	get_type = _gtk.gtk_vpaned_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_vpaned_new()

class GtkScrolledWindow(GtkBin):
	get_type = _gtk.gtk_scrolled_window_get_type
	def __init__(self, hadj=None, vadj=None, _obj=None):
		if _obj: self._o = _obj; return
		if hadj: hadj = hadj._o
		if vadj: vadj = vadj._o
		self._o = _gtk.gtk_scrolled_window_new(hadj, vadj)
	def __getattr__(self, attr):
		attrs = {
			'hscrollbar': _gtk.gtk_scrolled_window_get_hscrollbar,
			'vscrollbar': _gtk.gtk_scrolled_window_get_vscrollbar
		}
		if attrs.has_attr(attr):
			return _obj2inst(attrs[attr](self._o))
		else:
			return GtkBin.__getattr__(self, attr)
	def get_hadjustment(self):
		return _obj2inst(_gtk.gtk_scrolled_window_get_hadjustment(
			self._o))
	def get_vadjustment(self):
		return _obj2inst(_gtk.gtk_scrolled_window_get_vadjustment(
			self._o))
	def set_hadjustment(self, adjustment):
		_gtk.gtk_scrolled_window_set_hadjustment(self._o,adjustment._o)
	def set_vadjustment(self, adjustment):
		_gtk.gtk_scrolled_window_set_vadjustment(self._o,adjustment._o)
	def set_policy(self, hp, vp):
		_gtk.gtk_scrolled_window_set_policy(self._o, hp, vp)
	def set_placement(self, window_place):
		_gtk.gtk_scrolled_window_set_placement(self._o, window_place)
	def add_with_viewport(self, child):
		_gtk.gtk_scrolled_window_add_with_viewport(self._o, child._o)

class GtkSocket(GtkContainer):
	get_type = _gtk.gtk_socket_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_socket_new()
	def steal(self, wid):
		_gtk.gtk_socket_steal(self._o, wid)

class GtkTable(GtkContainer):
	get_type = _gtk.gtk_table_get_type
	def __init__(self, rows=1, cols=1, homogeneous=0, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_table_new(rows, cols, homogeneous)
	def attach(self, child, la, ra, ta, ba, xoptions=EXPAND|FILL,
			yoptions=EXPAND|FILL, xpadding=0, ypadding=0):
		_gtk.gtk_table_attach(self._o, child._o, la,ra,ta,ba,
			xoptions,yoptions, xpadding,ypadding)
	def set_row_spacing(self, row, s):
		_gtk.gtk_table_set_row_spacing(self._o, row, s)
	def set_col_spacing(self, col, s):
		_gtk.gtk_table_set_col_spacing(self._o, col, s)
	def set_row_spacings(self, s):
		_gtk.gtk_table_set_row_spacings(self._o, s)
	def set_col_spacings(self, s):
		_gtk.gtk_table_set_col_spacings(self._o, s)
	def set_homogeneous(self, homogeneous):
	        _gtk.gtk_table_set_homogeneous(self._o, homogeneous)
	def resize(self, rows, cols):
		_gtk.gtk_table_resize(self._o, rows, cols)

class GtkTree(GtkContainer):
	get_type = _gtk.gtk_tree_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_tree_new()
	def get_selection(self):
		return map(_obj2inst, _gtk.gtk_tree_get_selection(self._o))
	def append(self, child):
		_gtk.gtk_tree_append(self._o, child._o)
	def prepend(self, child):
		_gtk.gtk_tree_prepend(self._o, child._o)
	def insert(self, child, pos):
		_gtk.gtk_tree_insert(self._o, child._o, pos)
	def remove_item(self, child):
		# fix this when the function starts existing
		#_gtk.gtk_tree_remove_item(self._o, child._o)
		self.remove_items([child])
	def remove_items(self, children):
		children = map(lambda x: x._o, children)
		_gtk.gtk_tree_remove_items(self._o, children)
	def clear_items(self, start, end):
		_gtk.gtk_tree_clear_items(self._o, start, end)
	def select_item(self, item):
		_gtk.gtk_tree_select_item(self._o, item)
	def unselect_item(self, item):
		_gtk.gtk_tree_unselect_item(self._o, item)
	def select_child(self, child):
		_gtk.gtk_tree_select_child(self._o, child._o)
	def unselect_child(self, child):
		_gtk.gtk_tree_unselect_child(self._o, child._o)
	def child_position(self, child):
		return _gtk.gtk_tree_child_position(self._o, child._o)
	def set_selection_mode(self, mode):
		_gtk.gtk_tree_set_selection_mode(self._o, mode)
	def set_view_mode(self, mode):
		_gtk.gtk_tree_set_view_mode(self._o, mode)
	def set_view_lines(self, flag):
		_gtk.gtk_tree_set_view_lines(self._o, flag)

class GtkToolbar(GtkContainer):
	get_type = _gtk.gtk_toolbar_get_type
	class __wrap_cb:
		def __init__(self, cb):
			self.cb = cb
		def __call__(self, widget):
			return self.cb(_obj2inst(widget))
	def __init__(self, orientation=ORIENTATION_HORIZONTAL,
		     style=TOOLBAR_ICONS, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_toolbar_new(orientation, style)
	def append_element(self, type, widget, text, tooltip, tp, icon,
			   callback):
		if widget: widget = widget._o
		if icon: icon = icon._o
		if callback: callback = self.__wrap_cb(callback)
		return _obj2inst(_gtk.gtk_toolbar_append_element(
			self._o, type, widget, text, tooltip, tp, icon,
			callback))
        def append_item(self, text, tooltip, tp, icon, callback):
		if icon: icon = icon._o
		if callback: callback = self.__wrap_cb(callback)
		return _obj2inst(_gtk.gtk_toolbar_append_item(
			self._o, text, tooltip, tp, icon, callback))
	def append_space(self):
		_gtk.gtk_toolbar_append_space(self._o)
	def append_widget(self, w, tooltip, tp):
		_gtk.gtk_toolbar_append_widget(self._o, w._o, tooltip, tp)
	def insert_element(self, type, widget, text, tooltip, tp, icon,
			   callback, pos):
		if widget: widget = widget._o
		if icon: icon = icon._o
		if callback: callback = self.__wrap_cb(callback)
		return _obj2inst(_gtk.gtk_toolbar_insert_element(
			self._o, type, widget, text, tooltip, tp, icon,
			callback, pos))
	def insert_item(self, text, tooltip, tp, icon, callback, pos):
		if icon: icon = icon._o
		if callback: callback = self.__wrap_cb(callback)
		return _obj2inst(_gtk.gtk_toolbar_insert_item(
			self._o, text, tooltip, tp, icon, callback, pos))
	def insert_space(self, pos):
		_gtk.gtk_toolbar_insert_space(self._o, pos)
	def insert_widget(self, w, tooltip, tp, pos):
		_gtk.gtk_toolbar_insert_widget(self._o, w._o, tooltip, tp, pos)
	def prepend_element(self, type, widget, text, tooltip, tp, icon,
			    callback):
		if widget: widget = widget._o
		if icon: icon = icon._o
		if callback: callback = self.__wrap_cb(callback)
		return _obj2inst(_gtk.gtk_toolbar_prepend_element(
			self._o, type, widget, text, tooltip, tp, icon,
			callback))
	def prepend_item(self, text, tooltip, tp, icon, callback):
		if icon: icon = icon._o
		if callback: callback = self.__wrap_cb(callback)
		return _obj2inst(_gtk.gtk_toolbar_prepend_item(
			self._o, text, tooltip, tp, icon, callback))
	def prepend_space(self):
		_gtk.gtk_toolbar_prepend_space(self._o)
	def prepend_widget(self, w, tooltip, tp):
		_gtk.gtk_toolbar_prepend_widget(self._o, w._o, tooltip, tp)
       	def set_orientation(self, orientation):
		_gtk.gtk_toolbar_set_orientation(self._o, orientation)
	def set_style(self, style):
		_gtk.gtk_toolbar_set_style(self._o, style)
	def set_space_size(self, size):
		_gtk.gtk_toolbar_set_space_size(self._o, size)
	def set_space_style(self, style):
		_gtk.gtk_toolbar_set_space_style(self._o, style)
	def set_tooltips(self, enable):
		_gtk.gtk_toolbar_set_tooltips(self._o, enable)
	def set_button_relief(self, relief):
		_gtk.gtk_toolbar_set_button_relief(self._o, relief)
	def get_button_relief(self):
		return _gtk.gtk_toolbar_get_button_relief(self._o)

class GtkDrawingArea(GtkWidget):
	get_type = _gtk.gtk_drawing_area_get_type
	def __init__(self, _obj=None):
		self.__win = None
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_drawing_area_new()
	def size(self, w, h):
		_gtk.gtk_drawing_area_size(self._o, w, h)
	def __cache_win(self):
		self.realize()
		self.__win = self.get_window()
	def draw_point(self, gc, x, y):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_point(self.__win, gc, x, y)
	def draw_line(self, gc, x1, y1, x2, y2):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_line(self.__win, gc, x1, y1, x2, y2)
	def draw_rectangle(self, gc, fill, x, y, width, height):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_rectangle(self.__win, gc, fill, x, y,
					width, height)
	def draw_arc(self, gc, fill, x, y, width, height, angle1, angle2):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_arc(self.__win, gc, fill, x, y, width, height,
				  angle1, angle2)
	def draw_polygon(self, gc, fill, points):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_polygon(self.__win, gc, fill, points)
	def draw_string(self, font, gc, x, y, string):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_string(self.__win, font, gc, x, y, string)
	def draw_text(self, font, gc, x, y, text):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_text(self.__win, font, gc, x, y, text)
	def draw_pixmap(self, gc, src, xsrc,ysrc, xdest,ydest, width,height):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_pixmap(self.__win, gc, src, xsrc,ysrc,
				     xdest,ydest, width,height)
	def draw_points(self, gc, points):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_points(self.__win, gc, points)
	def draw_segments(self, gc, segs):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_segments(self.__win, gc, segs)
	def draw_lines(self, gc, points):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_lines(self.__win, gc, points)
	# these are the gdkrgb functions
	def draw_rgb_image(self, gc, x, y, width, height,
			   dither, buffer, rowstride):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_rgb_image(self.__win, gc, x, y, width, height,
					dither, buffer, rowstride)
	def draw_rgb_32_image(self, gc, x, y, width, height,
			      dither, buffer, rowstride):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_rgb_32_image(self.__win, gc, x, y, width, height,
					   dither, buffer, rowstride)
	def draw_gray_image(self, gc, x, y, width, height,
			    dither, buffer, rowstride):
		if not self.__win: self.__cache_win()
		_gtk.gdk_draw_gray_image(self.__win, gc, x, y, width, height,
					 dither, buffer, rowstride)

	# see the comment for draw_array closer to the bottom of this file.
	if hasattr(_gtk, "gdk_draw_array"):
		def draw_array(self, gc, x, y, dither, array):
			if not self.__win: self.__cache_win()
			_gtk.gdk_draw_array(self.__win, gc, x, y, dither,array)


class GtkCurve(GtkDrawingArea):
	get_type = _gtk.gtk_curve_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_curve_new()
	def reset(self):
		_gtk.gtk_curve_reset(self._o)
	def get_vector(self, size=-1):
		return _gtk.gtk_curve_get_vector(self._o, size)
	def set_vector(self, vector):
		_gtk.gtk_curve_set_vector(self._o, vector)
	def set_gamma(self, g):
		_gtk.gtk_curve_set_gamma(self._o, g)
	def set_range(self, minx,maxx, miny,maxy):
		_gtk.gtk_curve_set_range(self._o, minx,maxx, miny,maxy)
	def set_curve_type(self, tp):
		_gtk.gtk_curve_set_curve_type(self._o, tp)

class GtkEditable(GtkWidget):
	get_type = _gtk.gtk_editable_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def __getattr__(self, attr):
		attrs = {
		'selection_start_pos':
		_gtk.gtk_editable_get_selection_start_pos,
		'selection_end_pos':
		_gtk.gtk_editable_get_selection_end_pos,
		}
		if attrs.has_key(attr):
			return attrs[attr](self._o)
		return GtkWidget.__getattr__(self, attr)
	def select_region(self, start, end):
		_gtk.gtk_editable_select_region(self._o, start, end)
	def insert_text(self, new_text):
		return _gtk.gtk_editable_insert_text(self._o, new_text)
	def delete_text(self, start, end):
		_gtk.gtk_editable_delete_text(self._o, start, end)
	def get_chars(self, start, end):
		return _gtk.gtk_editable_get_chars(self._o, start, end)
	def cut_clipboard(self):
		_gtk.gtk_editable_cut_clipboard(self._o)
	def copy_clipboard(self):
		_gtk.gtk_editable_copy_clipboard(self._o)
	def paste_clipboard(self):
		_gtk.gtk_editable_paste_clipboard(self._o)
	def claim_selection(self, claim, time):
		_gtk.gtk_editable_claim_selection(self._o, claim, time)
	def delete_selection(self):
		_gtk.gtk_editable_delete_selection(self._o)
	def changed(self):
		_gtk.gtk_editable_changed(self._o)
	def set_position(self, pos):
		_gtk.gtk_editable_set_position(self._o, pos)
	def get_position(self):
		return _gtk.gtk_editable_get_position(self._o)
	def set_editable(self, is_editable):
		_gtk.gtk_editable_set_editable(self._o, is_editable)

class GtkEntry(GtkEditable):
	get_type = _gtk.gtk_entry_get_type
	def __init__(self, maxlen=-1, _obj=None):
		if _obj: self._o = _obj; return
		if maxlen == -1:
			self._o = _gtk.gtk_entry_new()
		else:
			self._o = _gtk.gtk_entry_new_with_max_length(maxlen)
	def set_text(self, text):
		_gtk.gtk_entry_set_text(self._o, text)
	def append_text(self, text):
		_gtk.gtk_entry_append_text(self._o, text)
	def prepend_text(self, text):
		_gtk.gtk_entry_prepend_text(self._o, text)
	def set_position(self, pos):
		_gtk.gtk_entry_set_position(self._o, pos)
	def get_text(self):
		return _gtk.gtk_entry_get_text(self._o)
	def select_region(self, start, stop):
		_gtk.gtk_entry_select_region(self._o, start, stop)
	def set_visibility(self, visible):
		_gtk.gtk_entry_set_visibility(self._o, visible)
	def set_editable(self, editable):
		_gtk.gtk_entry_set_editable(self._o, editable)
	def set_max_length(self, max):
		_gtk.gtk_entry_set_max_length(self._o, max)

class GtkSpinButton(GtkEntry):
	get_type = _gtk.gtk_spin_button_get_type
	def __init__(self, adj=None, climb_rate=1, digits=1, _obj=None):
		if _obj: self._o = _obj; return
		if adj:
			adj = adj._o
		self._o = _gtk.gtk_spin_button_new(adj, climb_rate, digits)
	def set_adjustment(self, adj):
		_gtk.gtk_spin_button_set_adjustment(self._o, adj._o)
	def get_adjustment(self):
		return _obj2inst(_gtk.gtk_spin_button_get_adjustment(self._o))
	def set_digits(self, digits):
		_gtk.gtk_spin_button_set_digits(self._o, digits)
	def get_value_as_float(self):
		return _gtk.gtk_spin_button_get_value_as_float(self._o)
	def get_value_as_int(self):
		return _gtk.gtk_spin_button_get_value_as_int(self._o)
	get_value = get_value_as_float
	def set_value(self, val):
		_gtk.gtk_spin_button_set_value(self._o, val)
	def set_update_policy(self, pol):
		_gtk.gtk_spin_button_set_update_policy(self._o, pol)
	def set_numeric(self, numeric):
		_gtk.gtk_spin_button_set_numeric(self._o, numeric)
	def spin(self, direction, step):
		_gtk.gtk_spin_button_spin(self._o, direction, step)
	def set_wrap(self, wrap):
	        _gtk.gtk_spin_button_set_wrap(self._o, wrap)
	def set_shadow_type(self, shadow_type):
		_gtk.gtk_spin_button_set_shadow_type(self._o, shadow_type)
	def set_snap_to_ticks(self, snap):
		_gtk.gtk_spin_button_set_snap_to_ticks(self._o, snap)
	def update(self):
		_gtk.gtk_spin_button_update(self._o)

class GtkText(GtkEditable):
	get_type = _gtk.gtk_text_get_type
	def __init__(self, hadj=None, vadj=None, _obj=None):
		if _obj: self._o = _obj; return
		if hadj: hadj = hadj._o
		if vadj: vadj = vadj._o
		self._o = _gtk.gtk_text_new(hadj, vadj)
	def set_editable(self, editable):
		_gtk.gtk_text_set_editable(self._o, editable)
	def set_word_wrap(self, word_wrap):
		_gtk.gtk_text_set_word_wrap(self._o, word_wrap)
	def set_line_wrap(self, line_wrap):
		_gtk.gtk_text_set_line_wrap(self._o, line_wrap)
	def set_adjustments(self, hadj, vadj):
		_gtk.gtk_text_set_adjustments(self._o, hadj._o, vadj._o)
	def get_hadjustment(self):
		return _obj2inst(_gtk.gtk_text_get_hadj(self._o))
	def get_vadjustment(self):
		return _obj2inst(_gtk.gtk_text_get_vadj(self._o))
	def set_point(self, point):
		_gtk.gtk_text_set_point(self._o, point)
	def get_point(self):
		return _gtk.gtk_text_get_point(self._o)
	def get_length(self):
		return _gtk.gtk_text_get_length(self._o)
	def freeze(self, obj=None):
		_gtk.gtk_text_freeze(self._o)
	def thaw(self, obj=None):
		_gtk.gtk_text_thaw(self._o)
	def insert(self, font, fg, bg, string):
		_gtk.gtk_text_insert(self._o, font, fg, bg, string,
				     len(string))
	def insert_defaults(self, chars):
		_gtk.gtk_text_insert_defaults(self._o, chars)
	def backward_delete(self, nchars):
		_gtk.gtk_text_backward_delete(self._o, nchars)
	def forward_delete(self, nchars):
		_gtk.gtk_text_forward_delete(self._o, nchars)

class GtkMisc(GtkWidget):
	get_type = _gtk.gtk_misc_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def set_alignment(self, xa, ya):
		_gtk.gtk_misc_set_alignment(self._o, xa, ya)
	def set_padding(self, xp, yp):
		_gtk.gtk_misc_set_padding(self._o, xp, yp)

class GtkArrow(GtkMisc):
	get_type = _gtk.gtk_arrow_get_type
	def __init__(self, at=ARROW_RIGHT, st=SHADOW_OUT, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_arrow_new(at, st)
	def set(self, at, st):
		_gtk.gtk_arrow_set(self._o, at, st)

class GtkPixmap(GtkMisc):
	get_type = _gtk.gtk_pixmap_get_type
	def __init__(self, parent_or_pixmap=None, xpm_or_mask=None, bg=None,
		     _obj=None):
		# This function can be called with two argument models:
		# The old model:
		#  GtkPixmap(parent_win, xpm_file, bg_colour)
		# or the one closer to the C version:
		#  GtkPixmap(pixmap, mask)
		if _obj: self._o = _obj; return
		if type(xpm_or_mask) == type(''):
			pix, mask = create_pixmap_from_xpm(parent_or_pixmap,
							   bg, xpm_or_mask)
			self._o = _gtk.gtk_pixmap_new(pix, mask)
		else:
			self._o = _gtk.gtk_pixmap_new(parent_or_pixmap,
						      xpm_or_mask)
	def get(self):
		return _gtk.gtk_pixmap_get(self._o)
	def set(self, pixmap, mask):
		_gtk.gtk_pixmap_set(self._o, pixmap, mask)
	def set_build_insensitive(self, build):
		_gtk.gtk_pixmap_set_build_insensitive(self._o, build)

class GtkLabel(GtkMisc):
	get_type = _gtk.gtk_label_get_type
	def __init__(self, label="", _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_label_new(label)
	def set(self, str):
		print "GtkLabel.set deprecated -- use set_text"
		self.set_text(str)
	def set_text(self, str):
		_gtk.gtk_label_set_text(self._o, str)
	def set_justify(self, jtype):
		_gtk.gtk_label_set_justify(self._o, jtype)
	def set_line_wrap(self, wrap):
		_gtk.gtk_label_set_line_wrap(self._o, wrap)
	def set_pattern(self, pattern):
		_gtk.gtk_label_set_pattern(self._o, pattern)
	def get(self):
		return _gtk.gtk_label_get(self._o)
	def parse_uline(self, string):
		return _gtk.gtk_label_parse_uline(self._o, string)

class GtkAccelLabel(GtkLabel):
	get_type = _gtk.gtk_accel_label_get_type
	def __init__(self, string=None, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_accel_label_new(string)
	def accelerator_width(self):
		print "accelerator_width deprecated -- use get_accel_width"
		return self.get_accel_width()
	def get_accel_width(self):
		return _gtk.gtk_accel_label_get_accel_width(self._o)
	def set_accel_widget(self, widget):
		_gtk.gtk_accel_label_set_accel_widget(self._o, widget._o)
	def refetch(self):
		_gtk.gtk_accel_label_refetch(self._o)

class GtkTipsQuery(GtkLabel):
	get_type = _gtk.gtk_tips_query_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_tips_query_new()
	def start_query(self, obj=None):
		_gtk.gtk_tips_query_start_query(self._o)
	def stop_query(self, obj=None):
		_gtk.gtk_tips_query_stop_query(self._o)
	def set_caller(self, caller):
		_gtk.gtk_tips_query_set_caller(self._o, caller._o)
	def set_labels(self, inactive, no_tip):
		_gtk.gtk_tips_query_set_labels(self._o, inactive, no_tip)

class GtkPreview(GtkWidget):
	get_type = _gtk.gtk_preview_get_type
	def __init__(self, type=PREVIEW_COLOR, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_preview_new(type)
	def size(self, w, h):
		_gtk.gtk_preview_size(self._o, w, h)
	def put(self, win, gc, srcx, srcy, dstx, dsty, width, height):
		_gtk.gtk_preview_put(self._o, win, gc, srcx, srcy,
				     dstx, dsty, width, height)
	def draw_row(self, data, x, y, w=None):
		_gtk.gtk_preview_draw_row(self._o, data, x, y, w or len(data))
	def set_expand(self, expand):
		_gtk.gtk_preview_set_expand(self._o, expand)
	def set_gamma(self, gamma):
		_gtk.gtk_preview_set_gamma(self._o, gamma)
	def set_color_cube(self, nr, ng, nb, ngrey):
		_gtk.gtk_preview_set_color_cube(self._o, nr,ng,nb, ngrey)
	def set_install_cmap(self, install):
		_gtk.gtk_preview_set_install_cmap(install)
	def set_reserved(self, reserved):
		_gtk.gtk_preview_set_reserved(reserved)
	def set_dither(self, dither):
		_gtk.gtk_preview_set_dither(self._o, dither)

class GtkProgress(GtkWidget):
	get_type = _gtk.gtk_progress_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def set_show_text(self, show_text):
		_gtk.gtk_progress_set_show_text(self._o, show_text)
	def set_text_alignment(self, x_align, y_align):
		_gtk.gtk_progress_set_text_alignment(self._o, x_align,
						     y_align)
	def set_format_string(self, format):
		_gtk.gtk_progress_set_format_string(self._o, format)
	def set_adjustment(self, adj):
		_gtk.gtk_progress_set_adjustment(self._o, adj._o)
	def reconfigure(self, value, min, max):
		print "reconfigure deprecated -- use configure"
		self.configure(value, min, max)
	def configure(self, value, min, max):
		_gtk.gtk_progress_configure(self._o, value, min, max)
	def set_percentage(self, pcnt):
		_gtk.gtk_progress_set_percentage(self._o, pcnt)
	def set_value(self, value):
		_gtk.gtk_progress_set_value(self._o, value)
	def get_value(self):
		return _gtk.gtk_progress_get_value(self._o)
	def set_activity_mode(self, activity_mode):
		_gtk.gtk_progress_set_activity_mode(self._o, activity_mode)
	def get_current_text(self):
		return _gtk.gtk_progress_get_current_text(self._o)
	def get_text_from_value(self, value):
		return _gtk.gtk_progress_get_text_from_value(self._o, value)
	def get_current_percentage(self):
		return _gtk.gtk_progress_get_current_percentage(self._o)
	def get_percentage_from_value(self, value):
		return _gtk.gtk_progress_get_percentage_from_value(self._o,
								   value)

class GtkProgressBar(GtkProgress):
	get_type = _gtk.gtk_progress_bar_get_type
	def __init__(self, adjustment=None, _obj=None):
		if _obj: self._o = _obj; return
		if adjustment:
			self._o = _gtk.gtk_progress_bar_new_with_adjustment(
				adjustment._o)
		else:
			self._o = _gtk.gtk_progress_bar_new()
	def set_bar_style(self, style):
		_gtk.gtk_progress_bar_set_bar_style(self._o, style)
	def set_discrete_blocks(self, blocks):
		_gtk.gtk_progress_bar_set_discrete_blocks(self._o, blocks)
	def set_activity_step(self, step):
		_gtk.gtk_progress_bar_set_activity_step(self._o, step)
	def set_activity_blocks(self, blocks):
		_gtk.gtk_progress_bar_set_activity_blocks(self._o, blocks)
	def set_orientation(self, orient):
		_gtk.gtk_progress_bar_set_orientation(self._o, orient)
	def update(self, pcnt):
		_gtk.gtk_progress_bar_update(self._o, pcnt)

class GtkRange(GtkWidget):
	get_type = _gtk.gtk_range_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def get_adjustment(self):
		return _obj2inst(_gtk.gtk_range_get_adjustment(self._o))
	def set_update_policy(self, pol):
		_gtk.gtk_range_set_update_policy(self._o, pol)
	def set_adjustment(self, adj):
		_gtk.gtk_range_set_adjustment(self._o, adj._o)
	def draw_background(self, obj=None):
		_gtk.gtk_range_draw_background(self._o)
	def clear_background(self, _obj=None):
		_gtk.gtk_range_clear_background(self._o)
	def draw_trough(self, obj=None):
		_gtk.gtk_range_draw_trough(self._o)
	def draw_step_forw(self, obj=None):
		_gtk.gtk_range_draw_step_forw(self._o)
	def draw_step_back(self, obj=None):
		_gtk.gtk_range_draw_step_back(self._o)

class GtkScale(GtkRange):
	get_type = _gtk.gtk_scale_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def set_digits(self, digits):
		_gtk.gtk_scale_set_digits(self._o, digits)
	def set_draw_value(self, draw_value):
		_gtk.gtk_scale_set_draw_value(self._o, draw_value)
	def set_value_pos(self, pos):
		_gtk.gtk_scale_set_value_pos(self._o, pos)
	def value_width(self):
		print "value_width deprecated -- use get_value_width"
		return self.get_value_width()
	def get_value_width(self):
		return _gtk.gtk_scale_get_value_width(self._o)
	def draw_value(self, obj=None):
		_gtk.gtk_scale_draw_value(self._o)

class GtkHScale(GtkScale):
	get_type = _gtk.gtk_hscale_get_type
	def __init__(self, adj=None, _obj=None):
		if _obj: self._o = _obj; return
		if adj: adj = adj._o
		self._o = _gtk.gtk_hscale_new(adj)

class GtkVScale(GtkScale):
	get_type = _gtk.gtk_vscale_get_type
	def __init__(self, adj=None, _obj=None):
		if _obj: self._o = _obj; return
		if adj: adj = adj._o
		self._o = _gtk.gtk_vscale_new(adj)

class GtkScrollbar(GtkRange):
	get_type = _gtk.gtk_scrollbar_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return

class GtkHScrollbar(GtkScrollbar):
	get_type = _gtk.gtk_hscrollbar_get_type
	def __init__(self, adj=None, _obj=None):
		if _obj: self._o = _obj; return
		if adj: adj = adj._o
		self._o = _gtk.gtk_hscrollbar_new(adj)

class GtkVScrollbar(GtkScrollbar):
	get_type = _gtk.gtk_vscrollbar_get_type
	def __init__(self, adj=None, _obj=None):
		if _obj: self._o = _obj; return
		if adj: adj = adj._o
		self._o = _gtk.gtk_vscrollbar_new(adj)

class GtkRuler(GtkWidget):
	get_type = _gtk.gtk_ruler_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
	def __getattr__(self, attr):
		if attr == 'position':
			return _gtk.gtk_ruler_get_position(self._o)
		else:
			return GtkWidget.__getattr__(self, attr)
	def set_metric(self, metric):
		_gtk.gtk_ruler_set_metric(self._o, metric)
	def set_range(self, lo, up, pos, max):
		_gtk.gtk_ruler_set_range(self._o, lo, up, pos, max)
	def draw_ticks(self, obj=None):
		_gtk.gtk_ruler_draw_ticks(self._o)
	def draw_pos(self, obj=None):
		_gtk.gtk_ruler_draw_pos(self._o)

class GtkHRuler(GtkRuler):
	get_type = _gtk.gtk_hruler_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_hruler_new()

class GtkVRuler(GtkRuler):
	get_type = _gtk.gtk_vruler_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_vruler_new()

class GtkSeparator(GtkWidget):
	get_type = _gtk.gtk_separator_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return

class GtkHSeparator(GtkSeparator):
	get_type = _gtk.gtk_hseparator_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_hseparator_new()

class GtkVSeparator(GtkSeparator):
	get_type = _gtk.gtk_vseparator_get_type
	def __init__(self, _obj=None):
		if _obj: self._o = _obj; return
		self._o = _gtk.gtk_vseparator_new()


_name2cls = {}
_supported = map(lambda x: eval(x), filter(lambda x: x[:3] == 'Gtk', dir()))
for m in _supported:
	_name2cls[m.__name__] = m
del m

class GtkAccelGroup:
	def __init__(self, _obj=None):
		if _obj: self._ag = _obj; return
		self._ag = _gtk.gtk_accel_group_new()
	def __cmp__(self, other):
	        if hasattr(other, '_ag'):
			return cmp(self._ag, other._ag)
		else:
			return cmp(id(self), id(other))
	def __hash__(self):
		return hash(self._ag)
	def activate(self, key, mods):
		_gtk.gtk_accel_group_activate(self._ag, key, mods)
	def attach(self, obj):
		_gtk.gtk_accel_group_attach(self._ag, obj._o)
	def detach(self, obj):
		_gtk.gtk_accel_group_detach(self._ag, obj._o)
	def lock(self):
		_gtk.gtk_accel_group_lock(self._ag)
	def unlock(self):
		_gtk.gtk_accel_group_unlock(self._ag)
	def lock_entry(self, key, mods):
		_gtk.gtk_accel_group_lock_entry(self._ag, key, mods)
	def unlock_entry(self, key, mods):
		_gtk.gtk_accel_group_unlock_entry(self._ag, key, mods)
	def add(self, key, mods, flags, obj, signal):
		_gtk.gtk_accel_group_add(self._ag, key, mods, flags,
					 obj._o, signal)
	def remove(self, key, mods, obj):
		_gtk.gtk_accel_group_remove(self._ag, key, mods, obj._o)

def _obj2inst(obj):
	objname = _gtk.gtk_type_name(_gtk.GTK_OBJECT_TYPE(obj))
	if _name2cls.has_key(objname):
		return _name2cls[objname](_obj=obj)
	# if I don't know what the object is, guess
	elif _gtk.GTK_CHECK_TYPE(obj, GtkBox.get_type()):
		return GtkBox(_obj=obj)
	elif _gtk.GTK_CHECK_TYPE(obj, GtkBin.get_type()):
		return GtkBin(_obj=obj)
	elif _gtk.GTK_CHECK_TYPE(obj, GtkWindow.get_type()):
		return GtkWindow(_obj=obj)
	elif _gtk.GTK_CHECK_TYPE(obj, GtkContainer.get_type()):
		return GtkContainer(_obj=obj)
	elif _gtk.GTK_CHECK_TYPE(obj, GtkWidget.get_type()):
		return GtkWidget(_obj=obj)
	else:
		return GtkObject(_obj=obj)

def _filtprops(props):
	for k in props.keys():
		v = props[k]
		if hasattr(v, '_o') and type(v._o) == _gtk.GtkObjectType:
			props[k] = v._o
		elif hasattr(v, '_ag') and type(v._ag)==_gtk.GtkAccelGroupType:
			props[k] = v._ag
		elif hasattr(v, '_im'):
			props[k] = v._im

def new(tp, **props):
	if type(tp) == type(GtkObject):
		tp = tp.get_type()
	elif type(tp) == type('forty-two'): 
		tp = _name2cls[tp].get_type()
	elif type(tp) != type(0):
		raise TypeError, "unknow type argument: " + tp
	_filtprops(props)
	return _obj2inst(_gtk.gtk_object_new(tp, props))

# flow control
def mainloop():
	_gtk.gtk_main()
def mainquit(*args):
	_gtk.gtk_main_quit()
def mainiteration(block=TRUE):
	return _gtk.gtk_main_iteration(block)
def events_pending():
	return _gtk.gtk_events_pending()

# idle/quit/timeout handlers
def idle_add(callback, *args):
	return _gtk.gtk_idle_add(callback, args)
def idle_add_priority(priority, callback):
	return _gtk.gtk_idle_add_priority(priority, callback)
def idle_remove(tag):
	_gtk.gtk_idle_remove(tag)
def quit_add(mainlevel, callback, *args):
        return _gtk.gtk_quit_add(mainlevel, callback, args)
def quit_add_destroy(mainlevel, object):
	_gtk.gtk_quit_add_destroy(mainlevel, object._o)
def quit_remove(tag):
	_gtk.gtk_quit_remove(tag)
def timeout_add(timeout, callback, *args):
	return _gtk.gtk_timeout_add(timeout, callback, args)
def timeout_remove(tag):
	_gtk.gtk_timeout_remove(tag)

# input (file handle) handler
def input_add(source, condition, callback):
	if hasattr(source, 'fileno'):
		# handle python file handles
		def wrapper(source, condition, real_s=source,real_cb=callback):
			real_cb(real_s, condition)
		return _gtk.gtk_input_add(source.fileno(), condition, wrapper)
	else:
		return _gtk.gtk_input_add(source, condition, callback)
def input_remove(tag):
	_gtk.gtk_input_remove(tag)

# these functions govern modality
def grab_add(wid):
	_gtk.gtk_grab_add(wid._o)
def grab_remove(wid):
	_gtk.gtk_grab_remove(wid._o)
def grab_get_current():
	return _obj2inst(_gtk.gtk_grab_get_current())

# RC parsing
def rc_parse(fname):
	_gtk.gtk_rc_parse(fname)
def rc_parse_string(string):
	_gtk.gtk_rc_parse_string(string)
def rc_reparse_all():
	_gtk.gtk_rc_reparse_all()

# font loading
def load_font(font):
	return _gtk.gdk_font_load(font)
def load_fontset(fontset):
	return _gtk.gdk_fontset_load(fontset)

# cursor loading
def cursor_new(type):
	return _gtk.gdk_cursor_new(type)
def cursor_new_from_pixmap(source, mask, fg, bg, x, y):
	return _gtk.gdk_cursor_new_from_pixmap(source, mask, fg, bg, x, y)

# colour allocation
def colour_alloc(parent, red, green=None, blue=None):
	print "colour_alloc deprecated.  Use GdkColormap.alloc"
	if hasattr(parent, '_o'): cmap = parent.get_window().colormap
	elif hasattr(parent, 'colormap'): cmap = parent.colormap
	else: cmap = parent
	if type(red) == type(''):
		return cmap.alloc(red)
	# red assumed to be an int
	if green == None and blue == None:
		green, blue = divmod(red, 256)
		red, green  = divmod(green, 256)
		red = red % 256

		red   = (red   + 1) * 256 - 1
		green = (green + 1) * 256 - 1
		blue  = (blue  + 1) * 256 - 1
	return cmap.alloc(red, green, blue)
# This one's for the Americans...
color_alloc = colour_alloc

# atoms ...
def atom_intern(name, only_if_exists=FALSE):
	return _gtk.gdk_atom_intern(name, only_if_exists)
def atom_name(atom):
	return _gtk.gdk_atom_name(atom)

# pixmap loading
def create_pixmap_from_xpm(object, bg, xpm):
	if type(object) == _gtk.GdkWindowType:
		return _gtk.gdk_pixmap_create_from_xpm(object, bg, xpm)
	else:
		if hasattr(object, 'get_colormap'):
			cmap = object.get_colormap()
		elif hasattr(object, 'colormap'):
			cmap = object.colormap
		else:
			cmap = object
		return _gtk.gdk_pixmap_colormap_create_from_xpm(None, cmap,
								bg, xpm)
# use XPM data ...
def create_pixmap_from_xpm_d(object, bg, xpm_d):
	if type(object) == _gtk.GdkWindowType:
		return _gtk.gdk_pixmap_create_from_xpm_d(object, bg, xpm_d)
	else:
		if hasattr(object, 'get_colormap'):
			cmap = object.get_colormap()
		elif hasattr(object, 'colormap'):
			cmap = object.colormap
		else:
			cmap = object
		return _gtk.gdk_pixmap_colormap_create_from_xpm_d(None, cmap,
								  bg, xpm_d)
def create_pixmap(window, width, height, depth=-1):
	if (hasattr(window, '_o')):
	        window.realize()
		window = window.get_window()
	return _gtk.gdk_pixmap_new(window, width, height, depth)

def create_bitmap_from_data(window, data, width, height):
	return _gtk.gdk_bitmap_create_from_data(window, data, width, height)

# drawing functions...
def draw_point(drawable, gc, x, y):
	_gtk.gdk_draw_point(drawable, gc, x, y)
def draw_line(drawable, gc, x1, y1, x2, y2):
	_gtk.gdk_draw_line(drawable, gc, x1, y1, x2, y2)
def draw_rectangle(drawable, gc, fill, x, y, width, height):
	_gtk.gdk_draw_rectangle(drawable, gc, fill, x, y,
				width, height)
def draw_arc(drawable, gc, fill, x, y, width, height, angle1, angle2):
	_gtk.gdk_draw_arc(drawable, gc, fill, x, y, width, height, angle1,
			  angle2)
def draw_polygon(drawable, gc, fill, points):
	_gtk.gdk_draw_polygon(drawable, gc, fill, points)
def draw_string(drawable, font, gc, x, y, string):
	_gtk.gdk_draw_string(drawable, font, gc, x, y, string)
def draw_text(drawable, font, gc, x, y, text):
	_gtk.gdk_draw_text(drawable, font, gc, x, y, text)
def draw_pixmap(drawable, gc, src, xsrc,ysrc, xdest,ydest, width,height):
	_gtk.gdk_draw_pixmap(drawable, gc, src, xsrc,ysrc,
			     xdest,ydest, width,height)
def draw_points(drawable, gc, points):
	_gtk.gdk_draw_points(drawable, gc, points)
def draw_segments(drawable, gc, segs):
	_gtk.gdk_draw_segments(drawable, gc, segs)
def draw_lines(drawable, gc, points):
	_gtk.gdk_draw_lines(drawable, gc, points)

# these are the gdkrgb functions
def draw_rgb_image(drawable, gc, x, y, width, height,
		   dither, buffer, rowstride):
	_gtk.gdk_draw_rgb_image(drawable, gc, x, y, width, height,
				dither, buffer, rowstride)
def draw_rgb_32_image(drawable, gc, x, y, width, height,
		   dither, buffer, rowstride):
	_gtk.gdk_draw_rgb_32_image(drawable, gc, x, y, width, height,
				   dither, buffer, rowstride)
def draw_gray_image(drawable, gc, x, y, width, height,
		   dither, buffer, rowstride):
	_gtk.gdk_draw_gray_image(drawable, gc, x, y, width, height,
				 dither, buffer, rowstride)

# this function renders an unsigned byte (type code 'b') numpy array to a
# drawable.  If it is a MxN or MxNx1 array, it is assumed to be a grey image.
# If it is a MxNx3 or MxNx4 array, it is assumed to be an rgb image.
# It should accept images that have been sliced along their first two axes,
# if you want to only draw part of the image.
if hasattr(_gtk, "gdk_draw_array"):
	def draw_array(drawable, gc, x, y, dither, array):
		_gtk.gdk_draw_array(drawable, gc, x, y, dither, array)


# screen size
def screen_width():
	return _gtk.gdk_screen_width()
def screen_height():
	return _gtk.gdk_screen_height()

def screen_width_mm():
	return _gtk.gdk_screen_width_mm()
def screen_height_mm():
	return _gtk.gdk_screen_height_mm()

# thread support -- place calls to these arround gtk code in threads ...
def threads_enter():
	_gtk.gdk_threads_enter()
def threads_leave():
	_gtk.gdk_threads_leave()

def gdk_char_width(font, char):
	return _gtk.gdk_char_width(font, char)

def gdk_flush():
	_gtk.gdk_flush()

def gdk_beep():
	_gtk.gdk_beep()

# these actually push/pop a visual/colormap pair.
def push_rgb_visual():
	_gtk.gtk_rgb_push_visual()
def pop_visual():
	_gtk.gtk_pop_visual()

# these are prefixed with underscores, since they are low level, and
# would break if pygtk was ported to windows.
def _window_foreign_new(xid):
	return _gtk.gdk_window_foreign_new(xid)
def _root_window():
	return _gtk.gdk_get_root_win()
def _window_new(parent, attribs):
	""" Creates a new gdk window which must be destroyed with an
	explicit call to the _destroy() method.  parent is the new 
	window's parent PyGdkWindow or None if the window should be a 
	child of the root window.  attribs must be a dictionary of 
	attribute values for the window; the following attribs must 
	be specified:
	  window_type -- GDK.WINDOW_TOPLEVEL, GDK.WINDOW_CHILD
	                 GDK.WINDOW_DIALOG, GDK.WINDOW_TEMP
	                 GDK.WINDOW_PIXMAP, or GDK.WINDOW_FOREIGN
	  event_mask -- the events to get
	  width -- width of window
	  height -- height of window
	The following attributes are optional:
	  wmclass -- GDK.INPUT_OUTPUT or GDK.INPUT_ONLY
	  x -- x offset from parent
	  y -- y offset from parent
	  colormap -- PyGdkColormap to use to interpret colors
	  override_redirect -- see gdk / X documentation
	"""
	
	return _gtk.gdk_window_new(parent, attribs)

_disable_gdk_threading = _gtk._disable_gdk_threading
