
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
 * $Id: gnome-compat.c,v 1.3 2009/08/15 00:04:55 erik Exp $
 */

/* all the gnome specific stuff should go in here. */

#include "config.h"

#include <stdio.h>

#include <gnome.h>

#include "bmud.h"
/*
#include "conf.h"
*/
#include "g-compat.h"
#include "gui.h"
#include "main.h"
#include "net.h"

void
init (int argc, char **argv)
{
    gnome_init ("BMUD", NULL, argc, argv);
    mud->window = gnome_app_new ("BMUD", _("Br0kEs MUD Client"));
}

void
do_about ()
{
    const char *authors[] = {
	"Erik Greenwald <erik@math.missouristate.edu> <erik@smluc.org>\n\tMain author/maintainer.",
	"Erik Thyren\n\tGTK+ connect wizard",
	"Robert J. Hansen <rjhansen@inav.net>\n\trewrite of net.c",
	"Dan Tenner\n\taliases",
	NULL
    };

    gtk_widget_show (gnome_about_new ("BMUD", VERSION,
	    "Copyright (c) 1998-2008 Erik Greenwald", _("Br0kEs mud client"),
	    authors, NULL, NULL, NULL));
}

void
do_menu (GtkWidget * garbage)
{
    GnomeUIInfo filemenu[] = {
	{
		GNOME_APP_UI_ITEM,
		N_("Connection Wizard"),
		N_("Connect to a mud"),
		(gpointer) clist,
		mud,
		NULL,
		GNOME_APP_PIXMAP_STOCK,
		GNOME_STOCK_MENU_NEW,
		0,
		0,
	    NULL},
	{
		GNOME_APP_UI_ITEM,
		N_("Connect..."),
		N_("Connect to a mud"),
		(gpointer) cbox,
		mud,
		NULL,
		GNOME_APP_PIXMAP_STOCK,
		GNOME_STOCK_MENU_NEW,
		0,
		0,
	    NULL},
	{
		GNOME_APP_UI_ITEM,
		N_("Disconnect"),
		N_("Disconnect from a mud"),
		(gpointer) disconnect,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_STOCK,
		GNOME_STOCK_MENU_CLOSE,
		0,
		0,
	    NULL},
	/*
	GNOMEUIINFO_SEPARATOR,
	{
		GNOME_APP_UI_ITEM,
		N_("Preferences..."),
		N_("Preferences"),
		(gpointer) show_config,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_STOCK,
		GNOME_STOCK_MENU_PREF,
		0,
		0,
	    NULL},
	*/
	GNOMEUIINFO_SEPARATOR,
	{
		GNOME_APP_UI_ITEM,
		N_("Exit"),
		N_("Exit bmud"),
		(gpointer) bmud_exit,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_STOCK,
		GNOME_STOCK_MENU_QUIT,
		0,
		0,
	    NULL},
	GNOMEUIINFO_END
    };

    GnomeUIInfo toolmenu[] = {

	{GNOME_APP_UI_ITEM, N_("Equip..."), N_("Equipment Window"), NULL, NULL,
	    NULL, GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_PROP, 0, 0, NULL},
	{GNOME_APP_UI_ITEM, N_("Map..."), N_("the mud map"), NULL, NULL, NULL,
	    GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_PREF, 0, 0, NULL},
	GNOMEUIINFO_END
    };

    GnomeUIInfo helpmenu[] = {

	{GNOME_APP_UI_ITEM, N_("About..."), NULL, (gpointer) do_about, NULL,
		NULL,
	    GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_ABOUT, 0, 0, NULL},
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_HELP ("bmud"),
	GNOMEUIINFO_END
    };

    GnomeUIInfo mainmenu[] = {
	GNOMEUIINFO_SUBTREE (N_("_File"), filemenu),
	GNOMEUIINFO_SUBTREE (N_("_Tools"), toolmenu),
	GNOMEUIINFO_SUBTREE (N_("_Help"), helpmenu),
	GNOMEUIINFO_END
    };

    gnome_app_create_menus ((GnomeApp *) mud->window, mainmenu);
    /*
     * put the Help menu on the far right 
     */
    gtk_menu_item_right_justify (GTK_MENU_ITEM (mainmenu[2].widget));

    /*
     * grey out the stuff that's not implemented 
     */
    /*
     * someday these will be with us... 
     */
    gtk_widget_set_sensitive (GTK_WIDGET (toolmenu[0].widget), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET (toolmenu[1].widget), FALSE);
    return;
}
