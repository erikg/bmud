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
 * $Id: alias_utils.c,v 1.3 2004/01/04 15:23:08 erik Exp $
 */

/*
 * This is mostly a macro layer between the Gtk Hashtable object and bmud,
 * so that we can use another hash table if we find a better one, without
 * changing the rest of the code... Also contains a few useful functions
 * to dump and load the table to/from a file, and to process the aliases
 * and output them.
 * ok, nevermind... it's the whole thing, nearly :-P
 * - DT
 */

	/* pulled out the #ifdef kludge and put this in automakes file -Erik */

#include <stdio.h>		/* BUFSIZ and other oopses */
#include <string.h>
#include <glib.h>

#include "bmud.h"
#include "alias.h"
#include "gui.h"
#include "net.h"

GHashTable *
alias_new ()
{
  return g_hash_table_new (g_str_hash, g_str_equal);
}

/* make this a fn.
#define alias_insert(table, key, value) g_hash_table_insert(table, key, value)
*/

#define alias_size(table) g_hash_table_size(table)

#define alias_lookup(table, key) g_hash_table_lookup(table, key)

#define alias_elookup(table, lookup_key, orig_key, value) g_hash_table_lookup_extended(table, lookup_key, orig_key, value)

#define alias_foreach(table, func, user_data) g_hash_table_foreach(table, func, user_data)

#define alias_remove(table, key) { \
	free ( alias_lookup(table, key) ); \
        g_hash_table_remove(table, key); \
        g_free ( key ); \
}

#define alias_freeze(table) g_hash_table_freeze(table)

#define alias_thaw(table) g_hash_table_thaw(table)

void
alias_destroy (GHashTable * table)
{
  alias_foreach (table, alias_free, NULL);
  g_hash_table_destroy (table);
  return;
}

/* called by alias_destroy(table) - user_data discarded.*/
void
alias_free (gpointer key, gpointer value, gpointer user_data)
{
  gchar *akey = (gchar *) key;
  gchar *avalue = (gchar *) value;
  (void) user_data;

  g_free (akey);
  g_free (avalue);
}

void
alias_insert (GHashTable * table, gchar * key, gchar * value)
{
  gchar *akey = g_strdup (key);
  gchar *avalue = g_strdup (value);

  /* clean it all up... */
  g_strstrip (akey);
  g_strstrip (avalue);

  /* get rid of those pesky \n's */
  g_strdelimit (akey, "\n", ';');
  g_strdelimit (avalue, "\n", ';');

  /* in case we're loading this from a file... */
  g_strdelimit (avalue, "|", ' ');

  /* now we can actually insert it, with no fear
     // of trying to free static strings when we destroy the table
     // later... */
  g_hash_table_insert (table, akey, avalue);
}


/*********************************************/

/* actual new functions!

// Dumps a single alias... to be called by alias_dump().
// The alias is dumped in the following format:
// alias/action\n
// so "/" is definitely a forbidden character for the aliases themselves!
// It's fine in the action, but not in the alias!*/
void
alias_dump_1 (gpointer key, gpointer value, gpointer user_data)
{
  gchar *akey = g_strdup ((gchar *) key);
  gchar *avalue = g_strdup ((gchar *) value);
  FILE *fp = (FILE *) user_data;

  /* clean it all up... better late than never! */
  g_strstrip (akey);
  g_strstrip (avalue);

  /* we don't want no \n's in our strings... otherwise it will
     // mess up when we read it all back in! */
  g_strdelimit (akey, "\n", ';');
  g_strdelimit (avalue, "\n", ';');

  /* Also: get rid of all spaces in value! (rep by |) */
  g_strdelimit (avalue, " ", '|');

  /* Now we're ready to fprintf this into fp! */
  fprintf (fp, "%s %s\n", akey, avalue);

  g_free (akey);
  g_free (avalue);

  /* That's all folks! */
}

void
alias_dump (gchar * filename, GHashTable * table)
{
  FILE *fp = fopen (filename, "w");

  if (fp == NULL)
    {
      g_warning ("Could not open file \"%s\" to save aliases!\n",
		 filename);
      return;
    }

  alias_foreach (table, alias_dump_1, fp);

  fclose (fp);

  g_message ("Aliases successfully saved to file \"%s\".", filename);
}

/* Note: GHashTable MUST be allocated already!*/
void
alias_load (gchar * filename, GHashTable * table)
{
  FILE *fp = fopen (filename, "r");
  gchar *line, *trig, *act;

  if (fp == NULL)
    {
      g_warning ("Could not open \"%s\" for read!!", filename);
      return;
    }

  /* We'll assume the max length of a line here is 10k... that 
     // should REALLY be plenty... */
  line = (gchar *) g_malloc0 (10 * 1024);
  trig = (gchar *) g_malloc0 (20);
  act = (gchar *) g_malloc0 (10 * 1024);

  while (!feof (fp))
    {
      fgets (line, 10 * 1024, fp);
      sscanf (line, "%s %s", trig, act);
      alias_insert (table, trig, act);
    }

  fclose (fp);
  g_free (line);
  g_free (trig);
  g_free (act);
}

/*/ does the substitution for $ arguments...*/
gchar *
alias_strsub (gchar * string, gchar ** blah_array)
{
  /* Max length of all substitutions together: BUFSIZ */
  gchar *output = g_malloc0 (sizeof (string) + BUFSIZ);
  guint m, n = 0;
  guint stringlen = strlen (string);
  guint outlen;

  while (n < stringlen)
    {
      if (string[n] == '$')
	{
	  m = string[n + 1] - 48;	/* ASCII nb->int conversion. 0=\48 */
	  if (blah_array[m])	/* to avoid segfault -Erik */
	    strcat (output, blah_array[m]);
	  n += 2;
	}
      else
	{
	  outlen = strlen (output);
	  output[outlen] = string[n];
	  n++;
	}
    }

  return output;
}

void
alias_process (GHashTable * table, gchar * blah)
{
  gchar *temp;
  gchar *akey;
  gchar *avalue;
  gchar **blah_array;
  gchar *output;

  if ((table == NULL) || (blah == NULL))
    return;

  /* split blah into a max of 10 tokens. */
  blah_array = g_strsplit (blah, " ", 10);

  akey = blah_array[0];
  avalue = alias_lookup (table, akey);

  g_message ("alias_process(): akey: %s", akey);
  g_message ("alias_process(): avalue: %s", avalue);

  if (avalue == NULL)
    {
      textfield_add ("Undefined Alias.\n", MESSAGE_NONE);
      return;
    }

  temp = g_strdup (avalue);

  output = alias_strsub (temp, blah_array);

  g_message (output);

  xmit_to_connection (output);	/* changed ssend to xmit -Erik */

  g_free (output);
  g_free (temp);
  g_strfreev (blah_array);
  return;
}

void
alias_trim_input (gchar * blah)
{
  guint n, count = 0;
  guint blah_len = strlen (blah);

  g_strstrip (blah);
  g_strdelimit (blah, "\n", ';');
  g_strdelimit (blah, " ", '|');

  for (n = 0; n < blah_len; n++)
    {
      if (blah[n] == '|')
	{
	  blah[n] = ' ';
	  count++;
	  if (count == 2)
	    return;
	}
    }
}
