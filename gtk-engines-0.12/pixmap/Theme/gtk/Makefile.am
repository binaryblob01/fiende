if MAINTAINER_MODE
png_files := $(shell cd $(srcdir); echo *.png)

# depends on $(theme_pngs) to handle deleted files
Makefile.am: Makefile.am.in $(png_files) $(theme_pngs)
	cd $(srcdir); \
	cp Makefile.am.in makam-temp; \
	echo 'theme_pngs = $(png_files)' >> makam-temp; \
	mv makam-temp Makefile.am

# handle deleted files, by forcing a rebuild
$(theme_pngs): 

endif

EXTRA_DIST = $(theme_DATA) Makefile.am.in

themedir = $(datadir)/themes/Pixmap/gtk

theme_DATA = gtkrc $(theme_pngs)

theme_pngs = arrow_down1.png arrow_down2.png arrow_down3.png arrow_left1.png arrow_left2.png arrow_left3.png arrow_right1.png arrow_right2.png arrow_right3.png arrow_up1.png arrow_up2.png arrow_up3.png brown_bg1.png brown_bg2.png button1.png button2.png button3.png button4.png button5.png button6.png button7.png button8.png button9.png button_def.png check1.png check2.png curve_bg1.png entry1.png entry2.png extension_bottom1.png extension_left1.png extension_right1.png extension_top1.png focus.png frame1.png frame2.png frame_gap.png frame_gap_top_end.png frame_gap_top_start.png gap_bottom_end.png gap_bottom.png gap_bottom_start.png gap_left_end.png gap_left.png gap_left_start.png gap_right_end.png gap_right.png gap_right_start.png gap_top_end.png gap_top.png gap_top_start.png handle_vert_thumb.png hline.png menubar.png notebook1.png option1.png option2.png option_menu.png progressbar.png ruler_horiz.png ruler_vert.png scrollbar_horiz1.png scrollbar_thumb2.png scrollbar_thumb.png scrollbar_vert1.png shadow1.png shadow2.png trough.png vline.png
