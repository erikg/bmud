
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
 * $Id: alias.h,v 1.3 2004/01/18 15:43:14 erik Exp $
 */

extern void do_alias (char *);
extern void free_aliases ();
extern GHashTable *alias_new ();
extern void alias_destroy (GHashTable *);
extern void alias_free (gpointer, gpointer, gpointer);
extern void alias_load (gchar *, GHashTable *);
extern void alias_dump (gchar *, GHashTable *);
extern void alias_trim_input (gchar *);
extern void alias_insert (GHashTable *, gchar *, gchar *);
extern void alias_process (GHashTable *, gchar *);
