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
 * $Id: gui.c,v 1.2 2003/11/19 20:12:52 erik Exp $
 */

/* this should handle the basic ui stuff that isn't handled by gnome? */

#include "bmud.h"
#include "color.h"
#include "gui.h"
#include "main.h"
#include "misc.h"
#include "net.h"
#include <stdio.h>

GtkWidget *win;

void
destructify ()
{
  gtk_widget_hide (win);
  return;
}

void
do_con ()
{
  char *port;
  char *host;

  port = (char *) g_malloc (10);
  host = (char *) g_malloc (1000);

  memset (port, 0, 10);
  memset (host, 0, 1000);

  host = gtk_entry_get_text (GTK_ENTRY (mud->hostentry));
  port = gtk_entry_get_text (GTK_ENTRY (mud->portentry));

  make_connection (host, port);
  destructify ();
  return;
}

void
cbox ()
{
  GtkWidget *hbox, *vbox, *label, *button;

  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_border_width (GTK_CONTAINER (win), 10);
  hbox = gtk_hbox_new (0, 5);
  vbox = gtk_vbox_new (0, 5);

  label = gtk_label_new (_("Host"));
  mud->hostentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), mud->hostentry, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (label);
  gtk_widget_show (mud->hostentry);
  gtk_widget_show (hbox);
  hbox = gtk_hbox_new (0, 0);

  label = gtk_label_new (_("Port"));
  mud->portentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), mud->portentry, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (label);
  gtk_widget_show (mud->portentry);
  gtk_widget_show (hbox);
  hbox = gtk_hbox_new (TRUE, 0);

  button = gtk_button_new_with_label (_("Connect"));
/*	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);	
	gtk_widget_grab_default (GTK_WIDGET(button));
*/
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (do_con), &mud);
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  button = gtk_button_new_with_label (_("Cancel"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (destructify), NULL);
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_container_add (GTK_CONTAINER (win), vbox);
  gtk_widget_show (hbox);
  gtk_widget_show (vbox);
  gtk_widget_show (win);

  return;
}

	/* create the main window */
GtkWidget *
spawn_gui ()
{
  GtkWidget *hbox, *vscrollbar;

  mud->vbox = gtk_vbox_new (FALSE, 0);
  hbox = gtk_hbox_new (FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (mud->window), "delete_event",
		      GTK_SIGNAL_FUNC (bmud_exit), NULL);
  gtk_signal_connect (GTK_OBJECT (mud->window), "destroy",
		      GTK_SIGNAL_FUNC (bmud_exit), NULL);

  do_menu (mud->vbox);

#ifdef USE_ZVT
  mud->text = zvt_term_new();
#else
  mud->text = gtk_text_new (0, 0);
  gtk_text_set_word_wrap (GTK_TEXT (mud->text), TRUE);	/* wordwrap */
#endif

  gtk_widget_show (mud->text);

  gtk_box_pack_start (GTK_BOX (hbox), mud->text, TRUE, TRUE, 0);
  vscrollbar = gtk_vscrollbar_new (GTK_TEXT (mud->text)->vadj);
  gtk_box_pack_start (GTK_BOX (hbox), vscrollbar, FALSE, FALSE, 0);
  gtk_widget_show (vscrollbar);

  gtk_widget_show (hbox);
  gtk_box_pack_start (GTK_BOX (mud->vbox), hbox, TRUE, TRUE, 0);

  hbox = gtk_hbox_new (0, 0);

  mud->stat = gtk_text_new (0, 0);
  gtk_widget_set_usize (mud->stat, mud->statsize, 20);
  gtk_text_set_word_wrap (GTK_TEXT (mud->stat), FALSE);
  gtk_text_set_line_wrap (GTK_TEXT (mud->stat), FALSE);
  gtk_widget_show (mud->stat);
  gtk_box_pack_start (GTK_BOX (hbox), mud->stat, FALSE, TRUE, 0);

  mud->ent = gtk_entry_new ();
  gtk_signal_connect_object (GTK_OBJECT (mud->ent), "activate",
			     GTK_SIGNAL_FUNC (send_to_connection), NULL);
  gtk_signal_connect (GTK_OBJECT (mud->ent), "key_press_event",
		      GTK_SIGNAL_FUNC (hist_evt), mud->ent);
  gtk_widget_show (mud->ent);
  gtk_box_pack_start (GTK_BOX (hbox), mud->ent, TRUE, TRUE, 0);

  gtk_widget_show (hbox);
  gtk_box_pack_start (GTK_BOX (mud->vbox), hbox, FALSE, TRUE, 0);

  gtk_container_border_width (GTK_CONTAINER (mud->window), 1);
#ifdef USE_GNOME
  gnome_app_set_contents (GNOME_APP (mud->window), mud->vbox);
#else
  gtk_widget_show (mud->vbox);
  gtk_container_add (GTK_CONTAINER (mud->window), mud->vbox);
#endif

  return mud->window;
}

	/* this is busted. when it's used, and it hits that magic spot where
	 * it starts culling, it does a slow scroll up to the top after every
	 * textfield_add(), then snaps to the bottom. */
void
clear_backbuffer ()
{
  int n = gtk_text_get_length (GTK_TEXT (mud->text));

  if (mud->maxlines <= 0 || n < mud->maxlines)
    return;
  gtk_text_freeze (GTK_TEXT (mud->text));
  gtk_text_set_point (GTK_TEXT (mud->text), n - mud->maxlines);
  gtk_text_backward_delete (GTK_TEXT (mud->text), n - mud->maxlines);
  gtk_text_set_point (GTK_TEXT (mud->text), mud->maxlines);
  gtk_text_thaw (GTK_TEXT (mud->text));
}

	/* stick words in the boxes */
void
textfield_add (gchar * message, int colortype)
{
  GtkAdjustment *adj = GTK_TEXT (mud->text)->vadj;
  int scrolled_up = FALSE;
  int x, numbytes = strlen (message);

  if (message[0] == '\0')
    return;

#ifndef USE_ZVT
  if (adj->value < adj->upper - adj->page_size)
    {
      scrolled_up = TRUE;
      gtk_text_freeze (GTK_TEXT (mud->text));
    }

  if (colortype == MESSAGE_NONE || colortype == MESSAGE_NORMAL)
    gtk_text_insert (GTK_TEXT (mud->text), mud->disp_font, &color[7][1], NULL,
		     message, -1);
  if (colortype == MESSAGE_ERR)
    gtk_text_insert (GTK_TEXT (mud->text), mud->disp_font, &color[1][0], NULL,
		     message, -1);
#endif  
  if (colortype == MESSAGE_ANSI)
    {
      /* break the ansi into 2 parts, and do 'em */
      x = numbytes;

      if (mud->statsize != 0)
	if (message[numbytes - 2] == '>')
	  {
	    clear (0, GTK_TEXT (mud->stat));
	    while (message[x] != '\n')
	      x--;
	    disp_ansi (numbytes - x, (gchar *) & message[x + 1], mud->stat);
	    x--;
	    message[x] = 0;
	    mud->curr_color = color[7][1];
	  }

      disp_ansi (x, message, mud->text);
    }

/*  clear_backbuffer ();	*/

#ifndef USE_ZVT
  if (scrolled_up)
    gtk_text_thaw (GTK_TEXT (mud->text));
  else if (adj->value < adj->upper - (adj->page_size))
    gtk_adjustment_set_value (adj, adj->upper - adj->page_size);
#endif
  
  gtk_widget_grab_focus (GTK_WIDGET (mud->ent));
  return;
}


	/* n is the number of lines to NOT delete, if it's 0, delete 'em all */
void
clear (int n, GtkText * target)
{
	n=0;
  gtk_text_freeze (target);
  gtk_text_backward_delete (target, gtk_text_get_length (target));
  gtk_text_thaw (target);
}

void
cleartail ()
{
  g_print ("%d\n", g_list_length (GTK_TEXT (mud->text)->current_line));
}
