extern void make_connection (char *host, char *port);
extern void open_connection (const char *host, const char *port);
extern void send_to_connection (GtkWidget *, gpointer data);
extern void xmit_to_connection (gchar *);
extern void read_from_connection (gpointer, gint, GdkInputCondition);
extern void disconnect (void);
