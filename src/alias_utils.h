
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
 * $Id: alias_utils.h,v 1.2 2004/01/18 15:43:15 erik Exp $
 */

#ifndef ALIAS_UTIL_H
#define ALIAS_UTIL_H


GHashTable *alias_new ();
void alias_destroy (GHashTable * table);
void alias_free (gpointer key, gpointer value, gpointer user_data);
void alias_insert (GHashTable * table, gchar * key, gchar * value);
void alias_dump_1 (gpointer key, gpointer value, gpointer user_data);
void alias_dump (gchar * filename, GHashTable * table);
void alias_load (gchar * filename, GHashTable * table);
gchar *alias_strsub (gchar * string, gchar ** blah_array);
void alias_process (GHashTable * table, gchar * blah);
void alias_trim_input (gchar * blah);

#endif
