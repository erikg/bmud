extern GtkWidget *spawn_gui (void);
extern void textfield_add (gchar *, int);
extern void cbox (void);
extern void clist (void);
extern void clear (int, GtkText *);
extern void cleartail ();
extern void init (int, char **);
extern void do_menu (GtkWidget *);
extern void do_about ();
extern void destructify ();

extern GtkWidget *win;		/* for use in main.o */
