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
 * $Id: gui.h,v 1.4 2004/01/05 00:11:29 erik Exp $
 */

extern GtkWidget *spawn_gui (void);
extern void textfield_add (gchar *, int);
extern void cbox (void);
extern void clist (void);
extern void clear (int, GtkWidget *);
extern void cleartail ();
extern void init (int, char **);
extern void do_menu (GtkWidget *);
extern void do_about ();
extern void destructify ();

extern GtkWidget *win;		/* for use in main.o */
