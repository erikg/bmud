/* houses the main function. */

#include <stdio.h>
#include <glib.h>

#include "conf.h"

#include "alias.h"
#include "bmud.h"
#include "config.h"
#include "color.h"
#include "gui.h"
#include "main.h"
#include "misc.h"
#include "net.h"
#include "prefs.h"
#include "script.h"

bmud *mud;			/* icky global */

#ifdef USE_GNOME
#include "gnome-compat.h"
#else
#include "gtk-compat.h"
#endif

#include <locale.h>

void
bmud_exit ()
{
  free_aliases ();
  gtk_main_quit ();
  return;
}

int
main (int argc, char **argv)
{
  GtkStyle *style;
  GtkThemeEngine *eng;
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  mud = g_malloc (sizeof (bmud));
  mud->hist = g_malloc (sizeof (struct bmud_history));
	mud->hist->max=20;
  mud->hist->list = g_malloc (sizeof (gpointer) * (mud->hist->max + 1));

  init (argc, argv);		/* mud->window is now a window widget */
  /* gnome-compat.h or gtk-compat.h */

  check_my_dir ();		/* misc.c */

  if (show_conf_dir (""))	/* misc.c */
    gtk_rc_parse (show_conf_dir ("gtkrc"));

  gtk_widget_realize (mud->window);
  mud->window = spawn_gui ();	/* gui.c */

  session_load ();		/* prefs.c */

  gtk_widget_set_usize (mud->stat, mud->statsize, 20);
  if (mud->statsize == 0)
    gtk_widget_set_usize (mud->stat, 1, 20);

  color_load ();		/* color.c */
/*
  if (mud->disp_font)
    g_free (mud->disp_font);
*/
  printf("Loading font: %s\n", mud->disp_font_name);	fflush(stdout);
  mud->disp_font = gdk_font_load (mud->disp_font_name);	/* mem leak */
  printf("disp_font: %x\n", mud->disp_font);fflush(stdout);

  mud->curr_color = color[7][1];

  gtk_widget_show (mud->window);

#ifndef USE_ZVT  
  {
  
  gdk_window_set_background (GTK_TEXT (mud->text)->text_area, &color[0][0]);
  gdk_window_set_background (GTK_TEXT (mud->stat)->text_area, &color[0][0]);
  }
#endif

  script_init(argc, argv);	/* if guile is enabled, this doesn't return */
  gtk_main ();

  return 0;
}
