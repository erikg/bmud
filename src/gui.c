
/*****************************************************************************
 *    BMUD - Br0kEs MUD Client                                               *
 *                                                                           *
 *     Copyright (C) 1998-2005 Erik Greenwald <erik@smluc.org>               *
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
 * $Id: gui.c,v 1.31 2005/10/04 20:44:22 erik Exp $
 */

/* this should handle the basic ui stuff that isn't handled by gnome? */

#include <stdio.h>

#include "config.h"

#include "bmud.h"
#include "color.h"
#include "gui.h"
#include "main.h"
#include "misc.h"
#include "net.h"

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

    port = (char *)g_malloc (10);
    host = (char *)g_malloc (1000);

    memset (port, 0, 10);
    memset (host, 0, 1000);

    host = (char *)gtk_entry_get_text (GTK_ENTRY (mud->hostentry));
    port = (char *)gtk_entry_get_text (GTK_ENTRY (mud->portentry));

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

    label = gtk_label_new ("Host");
    mud->hostentry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), mud->hostentry, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
    gtk_widget_show (label);
    gtk_widget_show (mud->hostentry);
    gtk_widget_show (hbox);
    hbox = gtk_hbox_new (0, 0);

    label = gtk_label_new ("Port");
    mud->portentry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), mud->portentry, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
    gtk_widget_show (label);
    gtk_widget_show (mud->portentry);
    gtk_widget_show (hbox);
    hbox = gtk_hbox_new (TRUE, 0);

    button = gtk_button_new_with_label ("Connect");


    gtk_signal_connect (GTK_OBJECT (button), "clicked",
	GTK_SIGNAL_FUNC (do_con), &mud);
    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
    gtk_widget_show (button);

    button = gtk_button_new_with_label ("Cancel");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
	GTK_SIGNAL_FUNC (destructify), NULL);
    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
    gtk_widget_show (button);

    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
    gtk_container_add (GTK_CONTAINER (win), vbox);
    gtk_widget_show (hbox);
    gtk_widget_show (vbox);
    gtk_widget_show (win);

    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    gtk_widget_grab_default (GTK_WIDGET (button));

    return;
}

void
resize (GtkContainer * window, gpointer * crud)
{
    gtk_widget_get_size_request (GTK_WIDGET (window), &mud->width,
	&mud->height);
    session_save ();
}

	/*
	 * create the main window 
	 */
GtkWidget *
spawn_gui ()
{
    GtkWidget *hbox, *scrollbox;

    gtk_window_set_default_size (GTK_WINDOW (mud->window), mud->width,
	mud->height);

/* TODO causes a spin?
    gtk_signal_connect (GTK_OBJECT (mud->window), "check-resize", GTK_SIGNAL_FUNC (resize), NULL);
*/

    mud->vbox = gtk_vbox_new (FALSE, 0);

    gtk_signal_connect (GTK_OBJECT (mud->window), "delete_event",
	GTK_SIGNAL_FUNC (bmud_exit), NULL);
    gtk_signal_connect (GTK_OBJECT (mud->window), "destroy",
	GTK_SIGNAL_FUNC (bmud_exit), NULL);

    do_menu (mud->vbox);

    /*
     * the text box 
     */
    scrollbox = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrollbox),
	GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrollbox);

    mud->text = gtk_text_view_new ();
    gtk_widget_show (mud->text);
    gtk_container_add (GTK_CONTAINER (scrollbox), mud->text);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (mud->text), FALSE);
    GTK_WIDGET_UNSET_FLAGS (mud->text, GTK_CAN_FOCUS);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (mud->text), GTK_WRAP_WORD);
    gtk_widget_modify_base (GTK_WIDGET (mud->text), GTK_STATE_NORMAL,
	&color[0][0]);
    gtk_widget_modify_text (GTK_WIDGET (mud->text), GTK_STATE_NORMAL,
	&color[7][0]);

    gtk_box_pack_start (GTK_BOX (mud->vbox), scrollbox, TRUE, TRUE, 0);

    hbox = gtk_hbox_new (0, 0);

    /*
     * the stat box 
     */
    mud->stat = gtk_text_view_new ();
    gtk_widget_show (mud->stat);
    gtk_widget_set_size_request (mud->stat, mud->statsize, 20);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (mud->stat), FALSE);
    GTK_WIDGET_UNSET_FLAGS (mud->text, GTK_CAN_FOCUS);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (mud->text), GTK_WRAP_NONE);
    gtk_widget_modify_base (GTK_WIDGET (mud->stat), GTK_STATE_NORMAL,
	&color[0][0]);
    gtk_widget_modify_text (GTK_WIDGET (mud->stat), GTK_STATE_NORMAL,
	&color[7][0]);
    gtk_box_pack_start (GTK_BOX (hbox), mud->stat, FALSE, TRUE, 0);

    /*
     * the entry field 
     */
    mud->ent = gtk_entry_new ();
    gtk_signal_connect_object (GTK_OBJECT (mud->ent), "activate",
	GTK_SIGNAL_FUNC (send_to_connection), NULL);
    gtk_signal_connect (GTK_OBJECT (mud->ent), "key_press_event",
	GTK_SIGNAL_FUNC (hist_evt), mud->ent);
    gtk_widget_show (mud->ent);
    gtk_box_pack_start (GTK_BOX (hbox), mud->ent, TRUE, TRUE, 0);

    /*
     * and glue it together 
     */
    gtk_widget_show (hbox);
    gtk_box_pack_start (GTK_BOX (mud->vbox), hbox, FALSE, TRUE, 0);

    gtk_container_border_width (GTK_CONTAINER (mud->window), 1);
    gnome_app_set_contents (GNOME_APP (mud->window), mud->vbox);

    return mud->window;
}

	/*
	 * this is busted. when it's used, and it hits that magic spot where
	 * it starts culling, it does a slow scroll up to the top after every
	 * textfield_add(), then snaps to the bottom. 
	 */
void
clear_backbuffer ()
{

/*
    int n = gtk_text_get_length (GTK_TEXT_VIEW (mud->text));

    if (mud->maxlines <= 0 || n < mud->maxlines)
	return;
    gtk_text_freeze (GTK_TEXT_VIEW (mud->text));
    gtk_text_set_point (GTK_TEXT_VIEW (mud->text), n - mud->maxlines);
    gtk_text_backward_delete (GTK_TEXT_VIEW (mud->text), n - mud->maxlines);
    gtk_text_set_point (GTK_TEXT_VIEW (mud->text), mud->maxlines);
    gtk_text_thaw (GTK_TEXT_VIEW (mud->text));
*/
}

void
textfield_add_ansi (GtkTextBuffer * buffer, char *text, GtkTextIter * iter,
    color_tag_t * tags)
{
    GtkTextIter start, end;
    int offset = gtk_text_iter_get_offset (iter);

    gtk_text_buffer_insert (buffer, iter, text, -1);
    while (tags)
    {
	color_tag_t *tmp;

	gtk_text_buffer_get_iter_at_offset (buffer, &start,
	    tags->start + offset);
	gtk_text_buffer_get_iter_at_offset (buffer, &end, tags->end + offset);
	gtk_text_buffer_apply_tag (buffer, &tags->tag, &start, &end);
	tmp = tags;
	tags = tags->next;
	free (tags);
    }
}

	/*
	 * stick words in the boxes 
	 */
void
textfield_add (gchar * message, int colortype)
{
    GtkTextIter iter, end;
    GtkTextMark *mark;
    GtkTextBuffer *buffer;
    GdkRectangle rect;

    int scrolled_up = FALSE;
    int y, height;

    if (!message || !*message)
	return;

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (mud->text));
    gtk_text_buffer_get_end_iter (buffer, &iter);

    mark = gtk_text_buffer_create_mark (buffer, NULL, &iter, FALSE);

/* stolen from gaim */
    gtk_text_view_get_visible_rect (GTK_TEXT_VIEW (mud->text), &rect);
    gtk_text_view_get_line_yrange (GTK_TEXT_VIEW (mud->text), &iter, &y,
	&height);

    if (((y + height) - (rect.y + rect.height)) > height
	&& gtk_text_buffer_get_char_count (buffer))
	scrolled_up = TRUE;

/* end of stolen chunk */

    switch (colortype)
    {
    case MESSAGE_NONE:
    case MESSAGE_NORMAL:
	gtk_text_buffer_insert (buffer, &iter, message, -1);
	break;
    case MESSAGE_ERR:
	/*
	 * TODO make this red 
	 */
	gtk_text_buffer_insert (buffer, &iter, message, -1);
	//gtk_text_insert (GTK_TEXT_VIEW (mud->text), mud->disp_font, &color[1][0], NULL, message, -1);
	break;
    case MESSAGE_ANSI:
	{
	    int x = 0, numbytes = strlen (message);
	    char *text = malloc (numbytes);
	    color_tag_t *tags;

	    /*
	     * break the ansi into 2 parts, and do 'em 
	     */
	    if (mud->statsize != 0 && message[numbytes - 2] == '>')
	    {
		GtkTextIter statiter;

		clear (0, mud->stat);
		while (message[x] != '\n')
		    x--;

/*
		disp_ansi (gtk_text_view_get_buffer (GTK_TEXT_VIEW (mud-> stat)), (gchar *) & message[x + 1], &statiter,
		    numbytes - x);
*/
		tags = disp_ansi (text, message);
		textfield_add_ansi (gtk_text_view_get_buffer (GTK_TEXT_VIEW
			(mud->stat)), text, &iter, tags);

		x--;
		message[x] = 0;
		mud->curr_color = color[7][1];
	    } else
	    {
		tags = disp_ansi (text, message);
		textfield_add_ansi (buffer, text, &iter, tags);
	    }
	    break;
	}
    default:
	textfield_add ("Unknown type!\n", MESSAGE_ERR);
    }

    clear_backbuffer ();

    if (!scrolled_up)
	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (mud->text), mark, 0, TRUE,
	    0.0, 1.0);
    gtk_widget_grab_focus (GTK_WIDGET (mud->ent));
    return;
}


	/*
	 * n is the number of lines to NOT delete, if it's 0, delete 'em all 
	 */
void
clear (int n, GtkWidget * target)
{
    gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW
	    (target)), "", -1);
}
