extern void do_alias (char *);
extern void free_aliases ();
extern GHashTable *alias_new ();
extern void alias_destroy (GHashTable *);
extern void alias_free (gpointer, gpointer, gpointer);
extern void alias_load(gchar *, GHashTable *);
extern void alias_dump(gchar *, GHashTable *);
extern void alias_trim_input(gchar *);
extern void alias_insert(GHashTable *, gchar *, gchar *);
extern void alias_process(GHashTable *, gchar *);

