extern GdkColor *foreground;
extern GdkColor color[9][2];
extern gchar *cn[9][2];		/* names of colors */

extern void show_colorm (void);
extern void init_colors ();
extern void disp_ansi (int, gchar *, GtkWidget *);
extern int color_load ();
extern int color_default ();
extern int color_dump ();
extern int color_save ();
