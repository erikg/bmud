
/*****************************************************************************
 *    BMUD - Br0kEs MUD Client                                               *
 *                                                                           *
 *     Copyright (C) 1998-2010 Erik Greenwald <erik@smluc.org>               *
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
 * $Id: misc.c,v 1.11 2010/01/06 01:05:59 erik Exp $
 */

/* stuff that doesn't go elsewhere. conf dir handling and history */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#ifdef USE_GNOME
# ifndef GNOME_H
#  include <gnome.h>
# endif
#else
# include <gtk/gtk.h>
#endif
#include <gdk/gdkkeysyms.h>

#include "bmud.h"
#include "misc.h"
#include "main.h"
#include "gui.h"

static char *bmud_conf_dir;

char *
gethome ()
{
    return getenv ("HOME");
}

char *
gethome_misc (char *subdir)
{
    char *buf;
    int len;

    len = strlen (gethome ()) + strlen (subdir) + 2;
    buf = (char *)malloc (len);
    snprintf (buf, len, "%s/%s", gethome (), subdir);
    return buf;
}

char *
gethome_conf ()
{
    return gethome_misc (".bmud");
}

char *
gethome_conf_file (char *file)
{
    char *buf;
    int len;

    buf = gethome_conf ();
    len = strlen (buf);
    buf = realloc (buf, strlen (buf) + strlen (file) + 2);
    snprintf (buf + len + 1, strlen (file) + 1, "/%s", file);
    return buf;
}

void
hist_add (char *x)
{
    /*
     * abort if history is disabled or x is blank 
     */
    if (mud->hist->max < 1 || x[0] == 0)
	return;
    /*
     * free up 'lost' bit 
     */
    if (mud->hist->list && mud->hist->list[mud->hist->pos])	/* make sure we don't free null */
	g_free (mud->hist->list[mud->hist->pos]);
    /*
     * add our line to the history 
     */
    mud->hist->list[mud->hist->pos] = g_strdup (x);
    /*
     * update the rest of history 
     */
    mud->hist->cur = mud->hist->pos;
    if (mud->hist->size < mud->hist->max)
	mud->hist->size++;
    mud->hist->pos = (mud->hist->pos + 1) % mud->hist->size;

    if (mud->hist->tmp)
	g_free (mud->hist->tmp);
    mud->hist->tmp = NULL;
    mud->hist->cur = mud->hist->pos;
}

void
hist_prev ()
{
    if (mud->hist->cyclic == 0)
	if ((mud->hist->pos + 1) % mud->hist->size == mud->hist->cur)
	    return;

/*      if(mud->hist->list[mud->hist->cur])	*/
    g_free (mud->hist->list[mud->hist->cur]);
    mud->hist->list[mud->hist->cur] =
	g_strdup (gtk_entry_get_text (GTK_ENTRY (mud->ent)));
    if (mud->hist->size > 0)
    {
	mud->hist->cur--;
	if (mud->hist->cur < 0)
	    mud->hist->cur = mud->hist->size - 1;
    }
    gtk_entry_set_text (GTK_ENTRY (mud->ent), mud->hist->list[mud->hist->cur]);
}

void
hist_next ()
{
    if (mud->hist->cyclic == 0)
	if (mud->hist->cur == mud->hist->pos)
	    return;
    mud->hist->list[mud->hist->cur] =
	g_strdup (gtk_entry_get_text (GTK_ENTRY (mud->ent)));
    if (mud->hist->size)
	mud->hist->cur = (mud->hist->cur + 1) % mud->hist->size;
    gtk_entry_set_text (GTK_ENTRY (mud->ent), mud->hist->list[mud->hist->cur]);
}

void
hist_clear ()
{
    int x;

    for (x = 0; x < mud->hist->max; x++)
	g_free (mud->hist->list[x]);
    mud->hist->cur = mud->hist->pos = 0;
}

gint
hist_evt (GtkWidget * w, GdkEventKey * event, gpointer data)
{
    data = NULL;
    switch (event->keyval)
    {
    case GDK_Up:
	{
	    hist_prev ();
	    break;
	}
    case GDK_Down:
	{
	    hist_next ();
	    break;
	}
    case GDK_Page_Up:
	{

/*
	GtkAdjustment *adj = GTK_TEXT (mud->text)->vadj;
	gtk_adjustment_set_value (adj, adj->value - adj->page_size);
*/
	    break;
	}
    case GDK_Page_Down:
	{

/*
	GtkAdjustment *adj = GTK_TEXT (mud->text)->vadj;
	if (adj->value < adj->upper - adj->page_size)
	  gtk_adjustment_set_value (adj, adj->value + adj->page_size);
*/
	    break;
	}
    default:
	/*
	 * normal handling 
	 */
	return 0;
	break;
    }
    gtk_signal_emit_stop_by_name (GTK_OBJECT (w), "key_press_event");
    return 1;
}

void
check_my_dir ()
{
    const char *homedir;

    if (bmud_conf_dir)
	return;

    if (bmud_conf_dir)
	g_free (bmud_conf_dir);
    bmud_conf_dir = NULL;

    homedir = g_get_home_dir ();

    if (homedir == NULL || homedir[0] == 0)
    {
	homedir = (char *)getenv ("HOME");
    }

    if (homedir != NULL && homedir[0] != 0)
    {
	DIR *x;

	bmud_conf_dir = g_strconcat (homedir, "/.bmud/", NULL);

	x = opendir (bmud_conf_dir);
	if (x == NULL)
	    mkdir (bmud_conf_dir, 0755);
	else
	{
	    closedir (x);
	}

	return;
    } else
	bmud_conf_dir = NULL;

    textfield_add ("Could not find your home dir", MESSAGE_ERR);
    printf ("Could not find your home dir");
    fflush (0);
}

gchar *zibberwalkie;

char *
show_conf_dir (char *file)
{
    if (zibberwalkie)
	g_free (zibberwalkie);
    zibberwalkie = g_strconcat (bmud_conf_dir, file, NULL);
    return zibberwalkie;
}
