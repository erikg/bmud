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
 * $Id: bmud.h,v 1.3 2003/12/25 16:36:08 erik Exp $
 */

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
