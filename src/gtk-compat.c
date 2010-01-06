
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
 * $Id: gtk-compat.c,v 1.4 2010/01/06 01:05:59 erik Exp $
 */

/* this is for the stuff that gnome would handle if we were using gnome. */

#include "g-compat.h"

void
init (int argc, char **argv)
{
    gtk_init (&argc, &argv);
    gtk_rc_parse ("/etc/gtkrc");
    gtk_rc_parse ("~/gtkrc");	/* I don't think this works */
    mud->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
}

void
do_menu (GtkWidget * vbox)
{
    GtkWidget *menu_bar, *menu, *smenu;
    GtkWidget *mi;
    GtkWidget *handlebox;

    menu_bar = gtk_menu_bar_new ();

    menu = gtk_menu_item_new_with_label (_("File"));
    gtk_widget_show (menu);
    smenu = gtk_menu_new ();
    gtk_widget_show (smenu);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu), smenu);

    mi = gtk_menu_item_new_with_label (_("C-Wizard"));
    gtk_signal_connect (GTK_OBJECT (mi), "activate", GTK_SIGNAL_FUNC (clist),
	NULL);
    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);


    mi = gtk_menu_item_new_with_label (_("Connect"));
    gtk_signal_connect (GTK_OBJECT (mi), "activate", GTK_SIGNAL_FUNC (cbox),
	NULL);
    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);

    mi = gtk_menu_item_new_with_label (_("Disconnect"));
    gtk_signal_connect (GTK_OBJECT (mi), "activate",
	GTK_SIGNAL_FUNC (disconnect), NULL);
    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);

    mi = gtk_menu_item_new ();
    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);

    /*
    mi = gtk_menu_item_new_with_label (_("Preferences..."));
    gtk_signal_connect (GTK_OBJECT (mi), "activate",
	GTK_SIGNAL_FUNC (show_config), NULL);
    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);
    */

    mi = gtk_menu_item_new ();
    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);

    mi = gtk_menu_item_new_with_label (_("Exit"));
    gtk_signal_connect (GTK_OBJECT (mi), "activate",
	GTK_SIGNAL_FUNC (bmud_exit), NULL);
    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);
    gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), menu);

    menu = gtk_menu_item_new_with_label (_("Tools"));
    gtk_widget_show (menu);
    smenu = gtk_menu_new ();
    gtk_widget_show (smenu);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu), smenu);
    mi = gtk_menu_item_new_with_label (_("Equip"));
    gtk_signal_connect (GTK_OBJECT (mi), "activate", GTK_SIGNAL_FUNC (NULL),
	NULL);
    gtk_widget_set_sensitive (GTK_WIDGET (mi), FALSE);

    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);
    mi = gtk_menu_item_new_with_label (_("Map"));
    gtk_signal_connect (GTK_OBJECT (mi), "activate", GTK_SIGNAL_FUNC (NULL),
	NULL);
    gtk_widget_set_sensitive (GTK_WIDGET (mi), FALSE);

    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);
    gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), menu);

    menu = gtk_menu_item_new_with_label (_("Help"));
    gtk_widget_show (menu);
    gtk_menu_item_right_justify (GTK_MENU_ITEM (menu));
    smenu = gtk_menu_new ();
    gtk_widget_show (smenu);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu), smenu);
    mi = gtk_menu_item_new_with_label (_("About"));
    gtk_signal_connect (GTK_OBJECT (mi), "activate",
	GTK_SIGNAL_FUNC (do_about), NULL);
    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);
    mi = gtk_menu_item_new_with_label (_("Manual"));	/* I d'no about this one */

/*	gtk_signal_connect(GTK_OBJECT(mi),"activate",GTK_SIGNAL_FUNC(gtk_main_quit),NULL);	*/
    gtk_widget_show (mi);
    gtk_menu_append (GTK_MENU (smenu), mi);
    gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), menu);

    gtk_widget_show (menu_bar);

    handlebox = gtk_handle_box_new ();
    gtk_widget_show (handlebox);
    gtk_container_add (GTK_CONTAINER (handlebox), menu_bar);
    gtk_box_pack_start (GTK_BOX (vbox), handlebox, FALSE, FALSE, 0);
    return;
}

void
dest (GtkWidget * button, GtkWidget * win)
{
    gtk_widget_destroy (win);
    return;
}

void
do_about ()
{
    char *x;
    GtkWidget *win, *label, *button, *vbox;

    x = (char *)g_malloc0 (sizeof (char) * 500);
    sprintf (x, "BMUD %s\nby Erik Greenwald\n<erik@smluc.org>", VERSION);
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    vbox = gtk_vbox_new (0, 5);
    label = gtk_label_new (x);
    button = gtk_button_new_with_label ("Ok");
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
    gtk_widget_show (button);
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (win), vbox);
    gtk_widget_show (win);
    gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (dest),
	win);
    return;
}
