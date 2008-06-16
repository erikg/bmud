
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
 * $Id: alias.h,v 1.6 2008/06/16 10:16:05 erik Exp $
 */

void do_alias (char *);
void free_aliases ();
GHashTable *alias_new ();
void alias_destroy (GHashTable *);
void alias_free (gpointer, gpointer, gpointer);
void alias_load (gchar *, GHashTable *);
void alias_dump (gchar *, GHashTable *);
void alias_trim_input (gchar *);
void alias_insert (GHashTable *, gchar *, gchar *);
void alias_process (GHashTable *, gchar *);
