#ifndef BMUD_H
#define BMUD_H

#ifdef USE_GNOME
#ifndef GNOME_H
#include <gnome.h>
#endif
#else
#include <gtk/gtk.h>
#include <libintl.h>
#define _(String) gettext(String)
#define N_(String) (String)
#endif

/*#include <libintl.h>	GNU gettext, for i18n */
/*#define _(String) gettext (String)  already in?*/


#define MESSAGE_ERR     0
#define MESSAGE_NORMAL  1
#define MESSAGE_ANSI    2
#define MESSAGE_NONE    3

#ifndef bool
#define bool char
#endif

#define SERVERFILE "mudservers"

struct bmud_history
{
  gchar **list;			/* the actual history */
  gchar *tmp;			/* in case something was typed before traverssing hist */
  int size;			/* current number of elements */
  int pos;			/* where to add */
  int cur;			/* which is displayed */
  int max;			/* total number of elements :) */
  int cyclic;			/* controls cyclic vs linear history */
};

typedef struct
{
  GtkWidget *window, *menu, *text, *stat, *ent, *hostentry, *portentry, *vbox;

  GdkFont *disp_font;
  gchar *disp_font_name;
  GdkColor curr_color;

  struct bmud_history *hist;

  gint items;
  gint lines;
  gint maxlines;
  gint statsize;
}
bmud;

#endif
