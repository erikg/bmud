/*****************************************************************************
 *    BMUD - Br0kEs MUD Client                                               *
 *                                                                           *
 *     Copyright (C) 1998-2003 Erik Greenwald <erik@smluc.org>               *
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
 * $Id: conf.c,v 1.1 2004/01/04 16:00:06 erik Exp $
 */

/* this is the gui for the config box */

#include <stdlib.h>
#include <gtk/gtktreeview.h>
#include "bmud.h"
#include "main.h"
#include "color.h"
#include "prefs.h"

GtkWidget *config_win, *blah, *hbox;
GtkWidget *hist_cyc, *hist_max;
GtkWidget *misc_statsize;
GdkColor ccol[9][2];
GdkColor *current;
char *fntname;
void (*this_panel_cb) ();
void update_font (GtkWidget *);
void update_color (GtkWidget *);

void
apply_cb (GtkWidget * crap, gpointer * nothing)
{
  int x;
  if (this_panel_cb)
    this_panel_cb ();

  /* pack the new font */
  if (fntname != NULL)
    {
/*		if(mud->disp_font_name)
			g_free(mud->disp_font_name);
*/
      mud->disp_font_name = g_strdup (fntname);
    }

/*	g_free(mud->disp_font);	*/
  mud->disp_font = gdk_font_load (mud->disp_font_name);

  /* pack the colors */
  for (x = 0; x < 9; ++x)
    {				/* unrolled. 2 isn't 'nuff for a loop. */
      memcpy (&color[x][0], &ccol[x][0], sizeof (GdkColor));
      memcpy (&color[x][1], &ccol[x][1], sizeof (GdkColor));
    }

  color_save ();
  session_save ();

  return;
}

void
cancel_cb (GtkWidget * crap, gpointer * nothing)
{
  gtk_widget_destroy (config_win);
  return;
}

void
ok_cb (GtkWidget * crap, gpointer * nothing)
{
  apply_cb (crap, nothing);
  cancel_cb (crap, nothing);
  return;
}

void
confinit ()
{
  int x;

  if (fntname)
    g_free (fntname);
  fntname = g_strdup (mud->disp_font_name);

  for (x = 0; x < 9; ++x)
    {
      memcpy (&ccol[x][0], &color[x][0], sizeof (GdkColor));
      memcpy (&ccol[x][1], &color[x][1], sizeof (GdkColor));
    }

  return;
}

void
defaults_cb (GtkWidget * crap, gpointer * nothing)
{
  color_default ();
	/*** TODO: set the config pane to use the default ***/
  return;
}

	/*** TODO ***/
void
help_cb (GtkWidget * crap, gpointer * nothing)
{
  return;
}


/**************** color stuff *********************/

void
update_color (GtkWidget * crap)
{
  gdouble c[3] = { 0, 0, 0 };
  GdkColor *confcol;
  confcol = current;

  gtk_color_selection_get_color (GTK_COLOR_SELECTION (blah), c);

  confcol->red = 65535 * c[0];
  confcol->green = 65535 * c[1];
  confcol->blue = 65535 * c[2];

  return;
}

void
confcolor (GtkWidget * placeholder, GdkColor * confcol)
{
  gdouble c[3] = { 0, 0, 0 };
  gint x;
  placeholder = gtk_vbox_new (FALSE, 2);

  if (blah != NULL)
    {
      if (this_panel_cb)
	this_panel_cb ();
      gtk_widget_destroy (blah);
    }

  c[0] = (gdouble) confcol->red / 65536;
  c[1] = (gdouble) confcol->green / 65536;
  c[2] = (gdouble) confcol->blue / 65536;

  blah = gtk_color_selection_new ();
  gtk_color_selection_set_color (GTK_COLOR_SELECTION (blah), c);

/*
  for (x = 0; x < 8; ++x)
    GTK_COLOR_SELECTION (blah)->old_values[x] =
      GTK_COLOR_SELECTION (blah)->values[x];
*/

  gtk_widget_show (blah);
  gtk_box_pack_start (GTK_BOX (placeholder), blah, 0, 0, 2);
  gtk_widget_show (placeholder);
  gtk_box_pack_start (GTK_BOX (hbox), placeholder, 0, 0, 2);

  this_panel_cb = update_color;
  current = confcol;

  return;
}

/****************** font stuff ********************/

void
update_font (GtkWidget * crap)
{
  if (fntname)
    g_free (fntname);
  fntname =
    g_strdup (gtk_font_selection_get_font_name (GTK_FONT_SELECTION (blah)));
  return;
}

void
conffont ()
{
  if (blah)
    {
      if (this_panel_cb)
	this_panel_cb ();
      gtk_widget_destroy (blah);
    }
  blah = gtk_font_selection_new ();
  gtk_font_selection_set_font_name (GTK_FONT_SELECTION (blah), fntname);
  gtk_widget_show (blah);
  gtk_box_pack_start (GTK_BOX (hbox), blah, 0, 0, 2);
  this_panel_cb = update_font;
  return;
}

/********************** history shtuff *******************/

void
update_hist (GtkWidget * crap)
{
  int x;
  mud->hist->max =
    gtk_spin_button_get_value_as_int ((GtkSpinButton *) hist_max);
  mud->hist->cyclic = GTK_TOGGLE_BUTTON (hist_cyc)->active;

  free (mud->hist->list);
  mud->hist->list = g_malloc (sizeof (void *) * mud->hist->max);

  for (x = 0; x < mud->hist->max; x++)
    mud->hist->list[x] = NULL;
  return;
}

void
confhist ()
{
  GtkWidget *h, *l;
  if (blah)
    {
      if (this_panel_cb)
	this_panel_cb ();
      gtk_widget_destroy (blah);
    }
  blah = gtk_vbox_new (0, 5);
  hist_cyc = gtk_check_button_new_with_label ("cyclic history");
  gtk_toggle_button_set_active ((GtkToggleButton *) hist_cyc,
				mud->hist->cyclic);
  gtk_widget_show (hist_cyc);
  gtk_box_pack_start (GTK_BOX (blah), hist_cyc, FALSE, FALSE, 0);
  l = gtk_label_new ("history maximum");
  h = gtk_hbox_new (0, 5);
  gtk_widget_show (l);
  gtk_box_pack_start (GTK_BOX (h), l, FALSE, FALSE, 0);
  hist_max = gtk_spin_button_new ((GtkAdjustment *)
				  gtk_adjustment_new (mud->hist->max, 0, 1024,
						      1, 10, 1), 0, 0);
  gtk_widget_show (hist_max);
  gtk_box_pack_start (GTK_BOX (h), hist_max, FALSE, FALSE, 0);
  gtk_widget_show (h);
  gtk_box_pack_start (GTK_BOX (blah), h, FALSE, FALSE, 0);
  gtk_widget_show (blah);
  gtk_box_pack_start (GTK_BOX (hbox), blah, 0, 0, 2);
  this_panel_cb = update_hist;
  return;
}

/**************** misc stuff ******************/

void
update_misc (GtkWidget * crap)
{
  mud->statsize =
    gtk_spin_button_get_value_as_int ((GtkSpinButton *) misc_statsize);
  if (mud->statsize == 0)
    gtk_widget_set_usize (mud->stat, 1, 20);
  else
    gtk_widget_set_usize (mud->stat, mud->statsize, 20);
  return;
}

void
confmisc ()
{
  GtkWidget *h, *l;
  if (blah)
    {
      if (this_panel_cb)
	this_panel_cb ();
      gtk_widget_destroy (blah);
    }
  blah = gtk_vbox_new (0, 5);
  h = gtk_hbox_new (0, 5);
  l = gtk_label_new ("Statbox size");
  gtk_box_pack_start (GTK_BOX (h), l, FALSE, FALSE, 0);
  gtk_widget_show (l);
  misc_statsize = gtk_spin_button_new ((GtkAdjustment *)
				       gtk_adjustment_new (mud->statsize, 0,
							   1024, 1, 10, 1), 0,
				       0);
  gtk_box_pack_start (GTK_BOX (h), misc_statsize, FALSE, FALSE, 0);
  gtk_widget_show (misc_statsize);
  gtk_box_pack_start (GTK_BOX (blah), h, FALSE, FALSE, 0);
  gtk_widget_show (h);
  gtk_widget_show (blah);
  gtk_box_pack_start (GTK_BOX (hbox), blah, 0, 0, 2);
  this_panel_cb = update_misc;
  return;
}

/*************** config toplevel *************/

void
show_config ()
{
  int x, y;
  GtkWidget *ok, *apply, *cancel, *defaults, *help;
  GtkWidget *vbox, *hbox2;
  GtkWidget *tree, *treeitem, *subtree, *scrollbox;
#if 0
  confinit ();

  config_win = gtk_dialog_new ();
  gtk_widget_realize (config_win);
  gtk_widget_set_usize (config_win, 670, 440);
  blah = gtk_label_new ("BMUD!");

  vbox = gtk_vbox_new (FALSE, 5);
  hbox = gtk_hbox_new (FALSE, 5);
  hbox2 = gtk_hbox_new (TRUE, 5);

  ok = gtk_button_new_with_label ("Ok");
  apply = gtk_button_new_with_label ("Apply");
  cancel = gtk_button_new_with_label ("Cancel");
  defaults = gtk_button_new_with_label ("Defaults");
  help = gtk_button_new_with_label ("Help");

  gtk_signal_connect (GTK_OBJECT (ok), "clicked", GTK_SIGNAL_FUNC (ok_cb),
		      NULL);
  gtk_signal_connect (GTK_OBJECT (apply), "clicked",
		      GTK_SIGNAL_FUNC (apply_cb), NULL);
  gtk_signal_connect (GTK_OBJECT (cancel), "clicked",
		      GTK_SIGNAL_FUNC (cancel_cb), NULL);
  gtk_signal_connect (GTK_OBJECT (defaults), "clicked",
		      GTK_SIGNAL_FUNC (defaults_cb), NULL);
  gtk_signal_connect (GTK_OBJECT (help), "clicked", GTK_SIGNAL_FUNC (help_cb),
		      NULL);

  scrollbox = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrollbox),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_widget_set_usize (scrollbox, 150, 200);
  gtk_widget_show (scrollbox);

  tree = gtk_tree_new ();
  subtree = gtk_tree_new ();

  for (x = 0; x < 9; ++x)
    for (y = 0; y < 2; ++y)
      {
	treeitem = gtk_tree_item_new_with_label (cn[x][y]);
	gtk_tree_append (GTK_TREE_VIEW (subtree), treeitem);
	gtk_signal_connect (GTK_OBJECT (treeitem), "select",
			    GTK_SIGNAL_FUNC (confcolor), &ccol[x][y]);
	gtk_widget_show (treeitem);
      }

  treeitem = gtk_tree_item_new_with_label ("Colors");
  gtk_tree_append (GTK_TREE_VIEW (tree), treeitem);

  gtk_tree_item_set_subtree (GTK_TREE_ITEM (treeitem), subtree);
  gtk_widget_show (treeitem);

  treeitem = gtk_tree_item_new_with_label ("Font");
  gtk_tree_append (GTK_TREE_VIEW (tree), treeitem);
  gtk_signal_connect (GTK_OBJECT (treeitem), "select",
		      GTK_SIGNAL_FUNC (conffont), NULL);
  gtk_widget_show (treeitem);

  treeitem = gtk_tree_item_new_with_label ("History");
  gtk_tree_append (GTK_TREE_VIEW (tree), treeitem);
  gtk_signal_connect (GTK_OBJECT (treeitem), "select",
		      GTK_SIGNAL_FUNC (confhist), NULL);
  gtk_widget_show (treeitem);

  treeitem = gtk_tree_item_new_with_label ("Misc");
  gtk_tree_append (GTK_TREE_VIEW (tree), treeitem);
  gtk_signal_connect (GTK_OBJECT (treeitem), "select",
		      GTK_SIGNAL_FUNC (confmisc), NULL);
  gtk_widget_show (treeitem);

  gtk_widget_show (blah);

  gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrollbox),
					 tree);

  gtk_box_pack_start (GTK_BOX (hbox), scrollbox, FALSE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX (hbox), blah, FALSE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX (hbox2), ok, FALSE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX (hbox2), apply, FALSE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX (hbox2), defaults, FALSE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX (hbox2), cancel, FALSE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX (hbox2), help, FALSE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX (vbox), hbox2, FALSE, TRUE, 2);

  gtk_widget_show (ok);
  gtk_widget_show (apply);
  gtk_widget_show (defaults);
  gtk_widget_show (cancel);
  gtk_widget_show (help);

  gtk_widget_show (tree);

  gtk_widget_show (hbox);
  gtk_widget_show (hbox2);
  gtk_widget_show (vbox);

  gtk_container_add (GTK_CONTAINER (config_win), vbox);

  gtk_widget_show (config_win);
#endif
  this_panel_cb = NULL;
  return;
}
