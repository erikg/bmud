
/*****************************************************************************
 *    BMUD - Br0kEs MUD Client                                               *
 *                                                                           *
 *     Copyright (C) 1998-2008 Erik Greenwald <erik@smluc.org>               *
 *                                                                           *
 *     This program is free software; you can redistribute it and/or modify  *
 *     it under the terms of the GNU General Public License as published by  *
 *     the Free Software Foundation; either version 2 of the License, or     *
 *     (at your option) any later version.                                   *
 *                                                                           *
 *     This program is distributed in the hope that it will be useful,       *
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *     GNU General Public License for more details.                          *
 *                                                                           *
 *     You should have received a copy of the GNU General Public License     *
 *     along with this program; if not, write to the Free Software           *
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.             *
 *                                                                           *
 *****************************************************************************/

/*
 * $Id: main.c,v 1.23 2009/08/15 00:04:55 erik Exp $
 */

/* houses the main function. */

#include "config.h"

#include <stdio.h>
#include <glib.h>


#ifdef USE_GNOME
# ifndef GNOME_H
#  include <gnome.h>
# endif
#else
# include <gtk/gtk.h>
#endif

#include "g-compat.h"

#include "alias.h"
#include "bmud.h"
#include "color.h"
/*
#include "conf.h"
*/
#include "gui.h"
#include "main.h"
#include "misc.h"
#include "net.h"
#include "prefs.h"
#include "script.h"

struct bmud_s *mud;			/* icky global */


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
    /*
    GtkStyle *style;
    GtkThemeEngine *eng;

    bindtextdomain (PACKAGE, LOCALEDIR);
    textdomain (PACKAGE);
    */

    mud = g_malloc (sizeof (struct bmud_s));
    memset (mud, 0, sizeof (struct bmud_s));
    mud->hist = g_malloc (sizeof (struct bmud_history));
    memset (mud->hist, 0, sizeof (struct bmud_history));
    mud->hist->max = 20;
    mud->hist->list = g_malloc (sizeof (gpointer) * (mud->hist->max + 1));

    init (argc, argv);		/* mud->window is now a window widget */

    check_my_dir ();		/* misc.c */
    if (show_conf_dir (""))	/* misc.c */
	gtk_rc_parse (show_conf_dir ("gtkrc"));

    session_load ();		/* prefs.c */
    color_load ();		/* color.c */

    gtk_widget_realize (mud->window);
    mud->window = spawn_gui ();	/* gui.c */

    if (mud->disp_font)
	g_free (mud->disp_font);
    mud->disp_font = gdk_font_load (mud->disp_font_name);	/* mem leak */

    mud->curr_color = color[7][1];

    gtk_widget_show (mud->window);
    gtk_widget_grab_focus (GTK_WIDGET (mud->ent));

    script_init (argc, argv);	/* if guile is enabled, this doesn't return */
    gtk_main ();

    return 0;
}
