#include <time.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gnome.h>
#include "minefield.h"
#include "flag.xpm"
#include "mine.xpm"

static struct {
	gint x;
	gint y;
} neighbour_map[8] = {
	{ -1,  1 },
	{ 0 ,  1 },
	{ 1 ,  1 },
	{ 1 ,  0 },
	{ 1 , -1 },
	{ 0 , -1 },
	{ -1, -1 },
	{ -1, 0  }
};

static int num_colors[9][3] = {
	{ 0  , 0  , 0   }, /* Black, not used */
	{ 0  , 0  , 255 }, /* Blue  */
	{ 0  , 160, 0   }, /* Green */
	{ 255, 0  , 0   }, /* Red   */
	{ 0  , 0  , 127 }, /* DarkBlue */
	{ 160, 0  , 0   }, /* DarkRed   */
	{ 0  , 255, 255 }, /* Cyan */
	{ 160, 0  , 160 }, /* DarkViolet */
	{ 0  , 0  , 0   }  /* Black */
};


time_t secs = 0;

enum {
	MARKS_CHANGED_SIGNAL,
	EXPLODE_SIGNAL,
	LOOK_SIGNAL,
	UNLOOK_SIGNAL,
	WIN_SIGNAL,
	LAST_SIGNAL
};

static gint minefield_signals[LAST_SIGNAL] = { 0 };

static GtkWidgetClass *parent_class;

static inline gint cell_idx (MineField *data, guint x, guint y)
{
	if (x >= 0 && x < data->xsize && y >= 0 && y < data->ysize)
		return x + y * data->xsize;
	return -1;
}

static void _setup_sign (sign *signp, char **data, guint minesize)
{
	GdkPixbuf *image;

        image = gdk_pixbuf_new_from_xpm_data ((const char **)data);
	gdk_pixbuf_render_pixmap_and_mask (image, &(signp->pixmap),
					   &(signp->mask), 128);

	gdk_pixbuf_unref (image);
        gdk_window_get_size(signp->pixmap, &(signp->width), &(signp->height));
}

static void gtk_minefield_setup_signs(GtkWidget *widget)
{
        GtkMineFieldView *mfield;
  
        mfield = GTK_MINEFIELD(widget);
  
        _setup_sign(&mfield->flag, flag_xpm, mfield->minesize);
        _setup_sign(&mfield->mine, mine_xpm, mfield->minesize);
}

static void gtk_minefield_realize(GtkWidget *widget)
{
        GtkMineFieldView *mfield;
        GdkWindowAttr attributes;
        gint attributes_mask;
        
        g_return_if_fail(widget != NULL);
        g_return_if_fail(GTK_IS_MINEFIELD (widget));
        
        mfield = GTK_MINEFIELD(widget);
        GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);
        
        attributes.window_type = GDK_WINDOW_CHILD;
        attributes.x = widget->allocation.x;
        attributes.y = widget->allocation.y;
        attributes.width = widget->allocation.width;
        attributes.height = widget->allocation.height;
        attributes.wclass = GDK_INPUT_OUTPUT;
        attributes.visual = gtk_widget_get_visual(widget);
        attributes.colormap = gtk_widget_get_colormap(widget);
        attributes.event_mask = gtk_widget_get_events(widget);
	attributes.event_mask |= GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK |
		                 GDK_BUTTON_RELEASE_MASK |
                                 GDK_POINTER_MOTION_MASK;
        
        attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
        
        widget->window = gdk_window_new(widget->parent->window, &attributes, attributes_mask);
        gdk_window_set_user_data(widget->window, mfield);
        
        widget->style = gtk_style_attach(widget->style, widget->window);
        gtk_style_set_background(widget->style, widget->window, GTK_STATE_ACTIVE);

        gtk_minefield_setup_signs(widget);

        mfield->cc = gdk_color_context_new (gtk_widget_get_visual (widget),
					    gtk_widget_get_colormap (widget));
}

static void gtk_minefield_unrealize (GtkWidget *widget)
{
	GtkMineFieldView *mfield;

	g_return_if_fail (widget != NULL);
	g_return_if_fail (GTK_IS_MINEFIELD (widget));

	mfield = GTK_MINEFIELD (widget);

	gdk_color_context_free (mfield->cc);
	mfield->cc = NULL;

	if (GTK_WIDGET_CLASS (parent_class)->unrealize)
		(* GTK_WIDGET_CLASS (parent_class)->unrealize) (widget);
}

static void gtk_minefield_size_allocate(GtkWidget *widget,
					GtkAllocation *allocation)
{
        g_return_if_fail(widget != NULL);
        g_return_if_fail(GTK_IS_MINEFIELD (widget));
        g_return_if_fail(allocation != NULL);

        widget->allocation = *allocation;
        
	if (GTK_WIDGET_REALIZED(widget)) {
		gdk_window_move_resize(widget->window,
				       allocation->x, allocation->y,
				       GTK_MINEFIELD(widget)->data->xsize *
				       GTK_MINEFIELD(widget)->minesize,
				       GTK_MINEFIELD(widget)->data->ysize *
				       GTK_MINEFIELD(widget)->minesize);
	}
}

static void gtk_minefield_size_request(GtkWidget *widget,
				       GtkRequisition *requisition)
{
        requisition->width  = GTK_MINEFIELD(widget)->data->xsize *
		              GTK_MINEFIELD(widget)->minesize;
	requisition->height = GTK_MINEFIELD(widget)->data->ysize * 
		              GTK_MINEFIELD(widget)->minesize;
}

static void gtk_mine_draw(GtkMineFieldView *mfield, guint x, guint y)
{
        int c = cell_idx (mfield->data, x, y);
	int noshadow;
	int n;
	guint minesize;

        GtkWidget *widget = GTK_WIDGET(mfield);

	minesize = mfield->minesize;

	if (mfield->data->lose || mfield->data->win) {
		noshadow = mfield->data->mines[c].shown;
	} else {
		noshadow = mfield->data->mines[c].down || mfield->data->mines[c].shown;
	}
	
	gdk_window_clear_area(widget->window,
			      x*minesize, y*minesize,
			      minesize,
			      minesize);

        if (!noshadow)
        {
                gtk_draw_shadow(widget->style, widget->window,
                                GTK_WIDGET_STATE (widget), GTK_SHADOW_OUT,
                                x*minesize, y*minesize,
                                minesize,
                                minesize);
        } else {
                gdk_draw_line(widget->window,
                              widget->style->black_gc,
                              x*minesize+minesize-1,
                              y*minesize,
                              x*minesize+minesize-1,
                              y*minesize+minesize-1);
                gdk_draw_line(widget->window,
                              widget->style->black_gc,
                              x*minesize,
                              y*minesize+minesize-1,
                              x*minesize+minesize-1,
                              y*minesize+minesize-1);
        }

	if (mfield->data->mines[c].shown && !mfield->data->mines[c].mined) {
		if ((n = mfield->data->mines[c].neighbours) != 0) {
			gdk_draw_string(widget->window,
					mfield->font,
					mfield->numstr[n].gc,
					x*minesize+mfield->numstr[n].dx,
					y*minesize+mfield->numstr[n].dy,
					mfield->numstr[n].text);
		}
	} else if (mfield->data->mines[c].marked == 1) {
		gdk_gc_set_clip_mask(widget->style->black_gc,
					     mfield->flag.mask);
		gdk_gc_set_clip_origin(widget->style->black_gc,
					       x * minesize + (minesize - mfield->flag.width) / 2, 
					       y * minesize + (minesize - mfield->flag.height) / 2);
		gdk_draw_pixmap (widget->window,
				 widget->style->black_gc,
				 mfield->flag.pixmap,
				 0, 0, x * minesize + (minesize - mfield->flag.width) / 2, 
				       y * minesize + (minesize - mfield->flag.height) / 2, -1, -1);
	        gdk_gc_set_clip_mask(widget->style->black_gc, NULL);
	  
		if (mfield->data->lose && mfield->data->mines[c].mined != 1) {
			gdk_draw_line(widget->window,
				      widget->style->black_gc,
				      x*minesize+2,
				      y*minesize+3,
				      x*minesize+minesize-4,
				      y*minesize+minesize-3);
			gdk_draw_line(widget->window,
				      widget->style->black_gc,
				      x*minesize+3,
				      y*minesize+2,
				      x*minesize+minesize-3,
				      y*minesize+minesize-4);
			gdk_draw_line(widget->window,
				      widget->style->black_gc,
				      x*minesize+2,
				      y*minesize+minesize-4,
				      x*minesize+minesize-4,
				      y*minesize+2);
			gdk_draw_line(widget->window,
				      widget->style->black_gc,
				      x*minesize+3,
				      y*minesize+minesize-3,
				      x*minesize+minesize-3,
				      y*minesize+3);
		}
	} else if ( mfield->data->lose && mfield->data->mines[c].mined) {
		if (mfield->mine.mask) {
			gdk_gc_set_clip_mask(widget->style->black_gc,
					     mfield->mine.mask);
			gdk_gc_set_clip_origin(widget->style->black_gc,
					       x*minesize + (minesize - mfield->mine.width) / 2, 
					       y*minesize + (minesize - mfield->mine.height) / 2);
		}

	        gdk_draw_pixmap (widget->window,
				 widget->style->black_gc,
				 mfield->mine.pixmap,
				 0, 0, x * minesize + (minesize - mfield->mine.width) / 2,
				       y * minesize + (minesize - mfield->mine.height) / 2, -1, -1);
		
		if (mfield->flag.mask) {
			gdk_gc_set_clip_mask(widget->style->black_gc, NULL);
		}
	}
}

static void gtk_minefield_draw(GtkMineFieldView *mfield, GdkRectangle *area)
{
	guint x1, y1, x2, y2, x, y, minesize;

	minesize = mfield->minesize;

	if (area) {
		x1 = area->x/minesize;
		y1 = area->y/minesize;
		x2 = (area->x + area->width - 1) / minesize;
		y2 = (area->y + area->height - 1) / minesize;
	} else {
		x1 = 0; y1 = 0;
		x2 = mfield->data->xsize;
		y2 = mfield->data->ysize;
	}
	
	for (x = x1; x <= x2; x++)
		for (y = y1; y <= y2; y++)
			gtk_mine_draw(mfield, x, y);
}

static gint gtk_minefield_expose(GtkWidget *widget,
				 GdkEventExpose *event)
{
        GtkMineFieldView *mfield;
	GdkColor color;
	gint n;
	guint minesize;
        int i;

	g_return_val_if_fail(widget != NULL, FALSE);
        g_return_val_if_fail(GTK_IS_MINEFIELD(widget), FALSE);
        g_return_val_if_fail(event != NULL, FALSE);

	if (!GTK_WIDGET_DRAWABLE(widget))
                return FALSE;

        mfield = GTK_MINEFIELD(widget);
	
	minesize = mfield->minesize;

	if (mfield->numstr[0].gc == 0) {
	        int pxlsz;
	        char fontname[50];
	  
	        gtk_minefield_setup_signs(widget);

	        pxlsz = minesize - 2;
	        if (pxlsz > 999) pxlsz = 999;
                if (pxlsz < 2)  pxlsz = 2;
  
                sprintf(fontname, "-bitstream-courier-bold-r-*-*-%d-*-*-*-*-*-*-*", pxlsz);
	
		mfield->font = gdk_font_load(fontname);
	  
	            /* The font used to be "-misc-fixed-bold-r-normal--13-*-*-*-*-*-*" */
	  
                if (!mfield->font) mfield->font = widget->style->font;
		for (i=0; i<9; i++) {
			mfield->numstr[i].text[0] = i+'0';
			mfield->numstr[i].text[1] = '\0';
			mfield->numstr[i].dx =
				(minesize-gdk_string_width(mfield->font,
							   mfield->numstr[i].text))/2;
			mfield->numstr[i].dy = (minesize + 5 * pxlsz / 8) / 2;
			mfield->numstr[i].gc = gdk_gc_new(GTK_WIDGET(mfield)->window);

			color.red   = num_colors[i][0] | (num_colors[i][0] << 8);
			color.green = num_colors[i][1] | (num_colors[i][1] << 8);
			color.blue  = num_colors[i][2] | (num_colors[i][2] << 8);
			color.pixel = 0; /* required! */

			n = 0;
			gdk_color_context_get_pixels (mfield->cc,
						      &color.red, &color.green, &color.blue,
						      1,
						      &color.pixel,
						      &n);

			gdk_gc_set_foreground(mfield->numstr[i].gc, &color);
		}
	}

	gtk_minefield_draw(GTK_MINEFIELD(widget), &event->area);

	return FALSE;
}

static inline int gtk_minefield_check_cell(GtkMineFieldView *mfield, guint x, guint y)
{
        guint changed;
        gint c;
        guint i;
        gint nx, ny;

        changed = 0;
        
        for (i=0; i<8; i++) {
                nx = x+neighbour_map[i].x;
                ny = y+neighbour_map[i].y;
                if ((c = cell_idx (mfield->data, nx, ny)) != -1) {
                        if (mfield->data->mines[c].shown == 0 &&
                            mfield->data->mines[c].marked == 0) {
				mfield->data->mines[c].shown = 1;
                                mfield->data->shown++;
				gtk_mine_draw(mfield, nx, ny);
				changed = 1;
			}
                }
        }
        return changed;
}


static void gtk_minefield_check_field(GtkMineFieldView *mfield, gint x, gint y)
{
        guint c;
        guint changed;

        gint x1, y1, x2, y2;
	gint cx1,cy1,cx2,cy2;
	
	cx1 = x-2;
	cy1 = y-2;
	cx2 = x+2;
	cy2 = y+2;

        do {
		x1 = cx1-1;
		y1 = cy1-1;
		x2 = cx2+1;
		y2 = cy2+1;

		if (x1 < 0) x1 = 0;
		if (y1 < 0) y1 = 0;
		if (x2 >= mfield->data->xsize) x2 = mfield->data->xsize-1;
		if (y2 >= mfield->data->ysize) y2 = mfield->data->ysize-1;
		
		changed = 0;
                for (x=x1; x<=x2; x++) {
                        for (y=y1; y<=y2; y++) {
                                c = cell_idx (mfield->data, x, y);
                                if (mfield->data->mines[c].neighbours == 0 &&
                                    mfield->data->mines[c].shown == 1) {
					changed |= gtk_minefield_check_cell(mfield, x, y);
					if (changed) {
						if (x < cx1) cx1 = x;
						if (x > cx2) cx2 = x;
						if (y < cy1) cy1 = y;
						if (y > cy2) cy2 = y;
					}
                                }
                        }
                }
        } while (changed);
}

static void gtk_minefield_loose(GtkMineFieldView *mfield)
{
        gtk_signal_emit(GTK_OBJECT(mfield),
                        minefield_signals[EXPLODE_SIGNAL]);
        mfield->data->lose = 1;
        gtk_minefield_draw(mfield, NULL);
}

static void gtk_minefield_win(GtkMineFieldView *mfield)
{
        guint x, y, c;

	for (x = 0; x < mfield->data->xsize; x++)
		for (y = 0; y < mfield->data->ysize; y++)
                {
                        c = x+y*mfield->data->xsize;
                        if (mfield->data->mines[c].shown == 0 && mfield->data->mines[c].marked == 0)
                        {
                                mfield->data->mines[c].marked = 1;
                                gtk_mine_draw(mfield, x, y);
                        }
                }

        gtk_signal_emit(GTK_OBJECT(mfield),
                        minefield_signals[WIN_SIGNAL]);
        mfield->data->win = 1;
}

static void gtk_minefield_show(GtkMineFieldView *mfield, guint x, guint y)
{
	int c = x+mfield->data->xsize*y;
	
        if (mfield->data->mines[c].marked != 1 && mfield->data->mines[c].shown != 1) {
		mfield->data->mines[c].shown = 1;
		mfield->data->shown++;
                gtk_mine_draw(mfield, mfield->cdownx, mfield->cdowny);
                if(mfield->data->mines[c].mined == 1) {
                        gtk_minefield_loose(mfield);
                } else {
			gtk_minefield_check_field(mfield, x, y);
			if (mfield->data->shown == mfield->data->xsize*mfield->data->ysize-mfield->data->mcount) {
                                gtk_minefield_win(mfield);
			}
		}
        }
}

static void gtk_minefield_toggle_mark(GtkMineFieldView *mfield, guint x, guint y)
{
        int c = cell_idx (mfield->data, x, y);
        if (mfield->data->mines[c].shown == 0) {
		if ((mfield->data->mines[c].marked = 1-mfield->data->mines[c].marked) == 1) {
			mfield->data->flags++;
		} else {
			mfield->data->flags--;
		}
		gtk_signal_emit(GTK_OBJECT(mfield),
				minefield_signals[MARKS_CHANGED_SIGNAL]);
                if (mfield->data->shown == mfield->data->xsize*mfield->data->ysize-mfield->data->mcount) {
                        gtk_minefield_win(mfield);
		}
        }
}

static inline void gtk_minefield_multi_press(GtkMineFieldView *mfield, guint x, guint y, gint c)
{
        guint i;
        gint nx, ny, c2;

        for (i=0; i<8; i++) {
                nx = x+neighbour_map[i].x;
                ny = y+neighbour_map[i].y;
                if ((c2 = cell_idx (mfield->data, nx, ny)) == -1)
                        continue;
                if (!mfield->data->mines[c2].marked && !mfield->data->mines[c2].shown) {
                        mfield->data->mines[c2].down = 1;
                        gtk_mine_draw(mfield, nx, ny);
                }
        }
        mfield->multi_mode = 1;
}

static void gtk_minefield_multi_release (GtkMineFieldView *mfield, guint x, guint y, guint c, guint really)
{
        gint n, nx, ny, i, c2;
        guint loose = 0;

        mfield->multi_mode = 0;

        n = 0;
        for (i=0; i<8; i++) {
                nx = x+neighbour_map[i].x;
                ny = y+neighbour_map[i].y;
                if ((c2 = cell_idx (mfield->data, nx, ny)) == -1)
                        continue;
                if (mfield->data->mines[c2].marked) n++;
        }
        if (mfield->data->mines[c].neighbours != n ||
            mfield->data->mines[c].marked ||
            !mfield->data->mines[c].shown) really = 0;

        for (i=0; i<8; i++) {
                nx = x+neighbour_map[i].x;
                ny = y+neighbour_map[i].y;
                if ((c2 = cell_idx (mfield->data, nx, ny)) == -1)
                        continue;
                if (mfield->data->mines[c2].down) {
                        mfield->data->mines[c2].down = 0;
                        if (really) {
				mfield->data->mines[c2].shown = 1;
				mfield->data->shown++;
                                if (mfield->data->mines[c2].mined == 1) {
                                        loose = 1;
                                }
                        }
                        gtk_mine_draw(mfield, nx ,ny);
                }
        }
        if (loose) {
                gtk_minefield_loose(mfield);
        } else if (really) {
                gtk_minefield_check_field(mfield, x, y);
                if (mfield->data->shown == mfield->data->xsize*mfield->data->ysize-mfield->data->mcount) {
			gtk_minefield_win(mfield);
		}
        }
}

static gint gtk_minefield_motion_notify(GtkWidget *widget, GdkEventMotion *event)
{
        GtkMineFieldView *mfield;
	guint x, y;
        guint c;
        guint multi;
	guint minesize;
	
        g_return_val_if_fail(widget != NULL, 0);
        g_return_val_if_fail(GTK_IS_MINEFIELD(widget), 0);
        g_return_val_if_fail(event != NULL, 0);

        mfield = GTK_MINEFIELD(widget);

	minesize = mfield->minesize;

        if (mfield->data->lose || mfield->data->win) return FALSE;

        if (mfield->bdown[0] || mfield->bdown[1]) {
                x = event->x/minesize;
                y = event->y/minesize;
                c = x+y*(mfield->data->xsize);

                if (c != mfield->cdown) {
                        mfield->data->mines[mfield->cdown].down = 0;
                        gtk_mine_draw(mfield, mfield->cdownx, mfield->cdowny);

                        multi = mfield->multi_mode;
                        if (multi) gtk_minefield_multi_release(mfield, mfield->cdownx, mfield->cdowny, mfield->cdown, 0);
                        mfield->cdownx = x;
                        mfield->cdowny = y;
                        mfield->cdown = c;
                        mfield->data->mines[c].down = 1;
                        gtk_mine_draw(mfield, x, y);

                        if (multi) gtk_minefield_multi_press(mfield, x, y, c);
                }
        }
        return FALSE;
}

static gint gtk_minefield_button_press(GtkWidget *widget, GdkEventButton *event)
{
        GtkMineFieldView *mfield;
	guint x, y;
	guint c;
	guint minesize;
        
        g_return_val_if_fail(widget != NULL, 0);
        g_return_val_if_fail(GTK_IS_MINEFIELD(widget), 0);
        g_return_val_if_fail(event != NULL, 0);

        mfield = GTK_MINEFIELD(widget);

	minesize = mfield->minesize;

	if (mfield->data->lose || mfield->data->win) return FALSE;
	
        if (event->button <= 3 && !mfield->bdown[1]) {
                x = event->x/minesize;
                y = event->y/minesize;
                c = x+y*(mfield->data->xsize);
                if (!mfield->bdown[0] && !mfield->bdown[1] && !mfield->bdown[2]) {
                        mfield->cdownx = x;
                        mfield->cdowny = y;
                        mfield->cdown = c;
                        mfield->data->mines[c].down = 1;
                }
		mfield->bdown[event->button-1]++;
                gtk_mine_draw(mfield, x, y);
                if (event->button == 2 || (mfield->bdown[0] && mfield->bdown[2])) { /* multi show */
                        gtk_minefield_multi_press(mfield, x, y, c);
                }
                else if (event->button == 3 && mfield->bdown[2] == 1)
                {
                        gtk_minefield_toggle_mark(mfield, x, y);
                        gtk_mine_draw(mfield, x, y);
                }
		if (event->button == 1 || event->button == 2) {
			gtk_signal_emit(GTK_OBJECT(mfield),
					minefield_signals[LOOK_SIGNAL]);
		}
        }
        return FALSE;
}

static gint gtk_minefield_button_release(GtkWidget *widget, GdkEventButton *event)
{
        GtkMineFieldView *mfield;

        g_return_val_if_fail(widget != NULL, FALSE);
        g_return_val_if_fail(GTK_IS_MINEFIELD(widget), FALSE);
        g_return_val_if_fail(event != NULL, FALSE);

	mfield = GTK_MINEFIELD(widget);

	if (mfield->data->lose || mfield->data->win) return FALSE;

        if (event->button <= 3 && mfield->bdown[event->button-1]) {
                if (mfield->bdown[0] && mfield->bdown[2] && event->button != 2) {
                    /* left+right click = multi show */
                    mfield->bdown[0] = 0;
                    mfield->bdown[1] = 1;
                    mfield->bdown[2] = 0;
                    event->button = 2;
                }
                switch (event->button) {
                case 1: gtk_minefield_show(mfield, mfield->cdownx, mfield->cdowny);
                        break;
                case 2: if (mfield->multi_mode) gtk_minefield_multi_release(mfield, mfield->cdownx, mfield->cdowny, mfield->cdown, 1);
                        break;
                }
		if (!mfield->data->lose && !mfield->data->win) {
			gtk_signal_emit(GTK_OBJECT(mfield),
					minefield_signals[UNLOOK_SIGNAL]);
		}
		mfield->data->mines[mfield->cdown].down = 0;
                mfield->cdown = -1;
		mfield->bdown[event->button-1] = 0;
		gtk_mine_draw(mfield, mfield->cdownx, mfield->cdowny);
        }
        return FALSE;
}


static void gtk_minefield_class_init (GtkMineFieldViewClass *class)
{
	GtkWidgetClass *widget_class;
	GtkObjectClass *object_class;
	
        widget_class = (GtkWidgetClass *)class;
	object_class = (GtkObjectClass *)class;

	parent_class = gtk_type_class (gtk_widget_get_type ());
	
        widget_class->realize = gtk_minefield_realize;
	widget_class->unrealize = gtk_minefield_unrealize;
        widget_class->size_allocate = gtk_minefield_size_allocate;
        widget_class->size_request = gtk_minefield_size_request;
        widget_class->expose_event = gtk_minefield_expose;
        widget_class->button_press_event = gtk_minefield_button_press;
	widget_class->button_release_event = gtk_minefield_button_release;
	widget_class->motion_notify_event = gtk_minefield_motion_notify;

        class->marks_changed = NULL;
        class->explode = NULL;
        class->look = NULL;
        class->unlook = NULL;
        class->win = NULL;
  
	minefield_signals[MARKS_CHANGED_SIGNAL] =
		gtk_signal_new("marks_changed",
			       GTK_RUN_FIRST,
			       object_class->type,
			       GTK_SIGNAL_OFFSET(GtkMineFieldViewClass, marks_changed),
			       gtk_signal_default_marshaller,
			       GTK_TYPE_NONE,
			       0);
	minefield_signals[EXPLODE_SIGNAL] =
		gtk_signal_new("explode",
			       GTK_RUN_FIRST,
			       object_class->type,
			       GTK_SIGNAL_OFFSET(GtkMineFieldViewClass, explode),
			       gtk_signal_default_marshaller,
			       GTK_TYPE_NONE,
			       0);
	minefield_signals[LOOK_SIGNAL] =
		gtk_signal_new("look",
			       GTK_RUN_FIRST,
			       object_class->type,
			       GTK_SIGNAL_OFFSET(GtkMineFieldViewClass, look),
			       gtk_signal_default_marshaller,
			       GTK_TYPE_NONE,
			       0);
	minefield_signals[UNLOOK_SIGNAL] =
		gtk_signal_new("unlook",
			       GTK_RUN_FIRST,
			       object_class->type,
			       GTK_SIGNAL_OFFSET(GtkMineFieldViewClass, unlook),
			       gtk_signal_default_marshaller,
			       GTK_TYPE_NONE,
			       0);
	minefield_signals[WIN_SIGNAL] =
		gtk_signal_new("win",
			       GTK_RUN_FIRST,
			       object_class->type,
			       GTK_SIGNAL_OFFSET(GtkMineFieldViewClass, win),
			       gtk_signal_default_marshaller,
			       GTK_TYPE_NONE,
			       0);

	gtk_object_class_add_signals(object_class, minefield_signals, LAST_SIGNAL);
}

static void gtk_minefield_init (GtkMineFieldView *mfield)
{
#ifndef GTK_HAVE_FEATURES_1_1_4
        GTK_WIDGET_SET_FLAGS (mfield, GTK_BASIC);
#endif
	mfield->cc = NULL;

        GTK_WIDGET (mfield)->requisition.width = mfield->minesize;
        GTK_WIDGET (mfield)->requisition.height = mfield->minesize;
}

void minefield_set_size (MineField *data, guint xsize, guint ysize)
{
        g_return_if_fail (data != NULL);

	if (data->xsize * data->ysize != xsize * ysize) {
		data->mines = g_realloc (data->mines,
					 sizeof(mine) * xsize * ysize);
	}
	data->xsize = xsize;
	data->ysize = ysize;
}

MineField *minefield_new ()
{
	MineField *data = g_new (MineField, 1);

	data->views  =  0;
	data->xsize  = -1;
	data->ysize  = -1;
	data->mcount =  0;
	data->mines  = NULL;
	data->flags  =  0;
	data->shown  =  0;
	data->lose   =  0;
	data->win    =  0;

	return data;
}

void minefield_destroy (MineField *data)
{
	g_return_if_fail (data != NULL);

	while (data->views)
		data->views = g_list_remove (data->views, data->views->data);

	g_free (data);
}

GtkWidget* gtk_minefield_new_view (MineField *data)
{
        GtkMineFieldView *mfield;
        
	mfield = gtk_type_new (gtk_minefield_get_type ());
	mfield->data = data;
	data->views = g_list_append (data->views, mfield);

	mfield->cdown = -1;
	mfield->numstr[0].gc = 0; /* Force GC generation */
	mfield->minesize = 17;

	return GTK_WIDGET (mfield);
}

guint gtk_minefield_get_type ()
{
        static guint minefield_type = 0;
        
        if (!minefield_type) {
                GtkTypeInfo minefield_info =
                        {
                                "GtkMineFieldView",
                                sizeof (GtkMineFieldView),
                                sizeof (GtkMineFieldViewClass),
                                (GtkClassInitFunc) gtk_minefield_class_init,
                                (GtkObjectInitFunc) gtk_minefield_init,
                                (GtkArgSetFunc) NULL,
                                (GtkArgGetFunc) NULL,
                        };
                        
                        minefield_type = gtk_type_unique (gtk_widget_get_type (),
							  &minefield_info);
        }
        
        return minefield_type;
}

void minefield_set_mines (MineField *data, guint mcount)
{
	GList *l;
        g_return_if_fail (data != NULL);
	
        data->mcount = mcount;
/*        data->numstr[0].gc = 0;*/

	l = data->views;
	while (l) {
		if (GTK_WIDGET_VISIBLE (l->data))
			gtk_widget_queue_resize (GTK_WIDGET (l->data));
		l = g_list_next (l);
	}
}

void gtk_minefield_set_minesize (GtkMineFieldView *mfield,
			     guint minesize)
{
	g_return_if_fail (mfield != NULL);

        mfield->minesize = minesize;
}

static gulong random_seed;

static void init_random(gulong seed)
{
        random_seed = seed;
}

static gulong get_random(gulong limit)
{
	do {
		random_seed = (random_seed*1139113+10921)>>2;
	} while (random_seed > ((gulong)(G_MAXLONG/limit))*limit);
	return random_seed % limit;
}

static void gtk_minefield_cleanup (GtkMineFieldView *mfield)
{
	mfield->bdown[0] = 0;
	mfield->bdown[1] = 0;
	mfield->bdown[2] = 0;
        mfield->cdown = -1;
        mfield->multi_mode = 0;
}

void minefield_set_at (MineField *data, int x, int y,
		       char num, char state)
{
	int idx;

	g_return_if_fail (data != NULL);

	idx = x + y * data->xsize;
	g_return_if_fail (idx < data->xsize * data->ysize);

	data->mines [idx].neighbours = num - '0';
	data->mines [idx].mined      = state == 'M';
	data->mines [idx].marked     = state == 'P';
	data->mines [idx].down       = state == 'v';
	data->mines [idx].shown      = state == ' ';
}

void minefield_get_at (MineField *data, int x, int y,
		       char *num, char *state)
{
	int idx;

	g_return_if_fail (data != NULL);

	idx = x + y * data->xsize;
	g_return_if_fail (idx < data->xsize * data->ysize);

	*num = data->mines [idx].neighbours + '0';

	if (data->mines [idx].mined)
		*state = 'M';
	else if (data->mines [idx].marked)
		*state = 'P';
	else if (data->mines [idx].down)
		*state = 'v';
	else if (data->mines [idx].shown)
		*state = ' ';
	else
		*state = '?';
}

void minefield_restart (MineField *data)
{
	guint i, j;
        guint x, y;
	guint tmp;
	guint n;
	guint cidx;
	GList *l;

        g_return_if_fail (data != NULL);

	data->flags = 0;
	data->shown = 0;
	data->lose  = 0;
	data->win   = 0;

	for (i=0; i<data->mcount; i++) {
		data->mines[i].mined = 1;
	}
	for (i = data->mcount; i < data->xsize * data->ysize; i++)
		data->mines[i].mined = 0;

	if (secs == 0) {
		time((time_t *)&secs);
		init_random(secs);
	}
	
	for (i = 0; i < data->xsize * data->ysize; i++) {
                data->mines[i].marked = 0;
                data->mines[i].shown  = 0;
		data->mines[i].down   = 0;
		j = (guint)get_random(data->xsize*data->ysize);
		tmp = data->mines[i].mined;
		data->mines[i].mined = data->mines[j].mined;
		data->mines[j].mined = tmp;
	}

	for (x = 0; x < data->xsize; x++) {
		for (y = 0; y < data->ysize; y++) {
			n = 0;
			for (i = 0; i < 8; i++) {
				if ((cidx = cell_idx (data, x + neighbour_map[i].x,
						      y + neighbour_map[i].y)) != -1) {
					if (data->mines[cidx].mined) n++;
				}
			}
                        data->mines[x + data->xsize * y].neighbours = n;
		}
	}

	l = data->views;
	while (l) {
		gtk_minefield_cleanup (l->data);
		l = g_list_next (l);
	}
}

