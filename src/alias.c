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
 * $Id: alias.c,v 1.3 2003/12/25 16:36:08 erik Exp $
 */

/* this is where the ircII style aliasing goes I thnk */

/* ok, the way these are gotten to is either have a '/' or a '#' at the
beginning of the entry line... */

#include <string.h>
#include <stdio.h>		/* for BUFSIZ */
#include <glib.h>
#include "alias.h"
#include "bmud.h"
#include "color.h"
#include "gui.h"
#include "main.h"
#include "net.h"
#include "prefs.h"

#define fake_help _("\
There is no real help yet. This is a quicky list of aliases\n\
/connect [hostname <port>]\tConnect to a server\n\
/disconnect\t\tDisconnect from the current server\n\n\
/quit\t\texit this program\n\
/exit\t\texit the program\n\n\
/aload\t\tload the alias file\n\
/asave\t\tsave the aliases to the alias file\n\
/afree\t\tflush all the aliases\n\
/alias <alias name> <commands>\tset an alias\n\n\
/cload\t\tload color file\n\
/csave\t\tsave color file\n\
/cdef\t\tload default colors\n\n\
/sload\t\tload session info\n\
/ssave\t\tsave session info\n\
/sdef\t\tuse default session stuff\n\
/help\t\tthis help thingy\n\n\
All the command /'s can be replaced with # if you want, either works.\n")


static GHashTable *alias_table;

void
fake_help_thingy ()
{
  textfield_add (fake_help, MESSAGE_NORMAL);
  return;
}

void
do_load_aliases (char *blah)
{
  char *trash, *filename;

  trash = g_malloc (1024);
  filename = g_malloc (1024);
  sscanf (blah, "%s %s", trash, filename);
  g_free (trash);

}

void
connect_alias (char *blah)
{
  char *trash, *host, *port;
  trash = (char *) g_malloc (1024);
  host = (char *) g_malloc (1024);
  port = (char *) g_malloc (1024);
  host[0] = 0;
  port[0] = 0;
  sscanf (blah, "%s %s %s", trash, host, port);
  g_free (trash);
  if (host[0] != 0 && port[0] != 0)
    {
      make_connection (host, port);
      g_free (host);
      g_free (port);
      return;
    }
  g_free (host);
  g_free (port);
  cbox ();
  return;
}

void
load_aliases (gchar * blah)
{
  gchar *trash;
  gchar *filename;

  trash = (gchar *) g_malloc0 (BUFSIZ);
  filename = (gchar *) g_malloc0 (BUFSIZ);

  sscanf (blah, "%s %s", trash, filename);

  g_free (trash);

  if (alias_table != NULL)
    {
      /* table has already been allocated. Must destroy
         to get rid of dangling pointers! */
      alias_destroy (alias_table);
    }

  alias_table = alias_new ();

  alias_load (filename, alias_table);

  textfield_add (_("Aliases loaded.\n"), MESSAGE_NONE);

  g_free (filename);
}

void
save_aliases (gchar * blah)
{
  gchar *trash;
  gchar *filename;

  /* if the alias_table doesn't exist, no point in doing all this! */
  if (alias_table == NULL)
    {
      textfield_add (_("There are not aliases to save!!\n"), MESSAGE_NONE);
      g_warning (_("Tried to save empty alias table!"));
      return;
    }

  trash = (gchar *) g_malloc0 (BUFSIZ);
  filename = (gchar *) g_malloc0 (BUFSIZ);

  sscanf (blah, "%s %s", trash, filename);

  g_free (trash);

  alias_dump (filename, alias_table);

  g_free (filename);
}

void
free_aliases ()
{
  if (alias_table != NULL)
    {
      alias_destroy (alias_table);
    }
  else
    {
      textfield_add (_("Alias table was already NULL!\n"), MESSAGE_NONE);
    }
  return;
}

void
add_alias (gchar * blah)
{
  gchar *trash;
  gchar *akey;
  gchar *avalue;

  if (alias_table == NULL)
    alias_table = alias_new ();

  trash = (gchar *) g_malloc (BUFSIZ);
  akey = (gchar *) g_malloc0 (BUFSIZ);
  avalue = (gchar *) g_malloc0 (BUFSIZ);

  alias_trim_input (blah);

  sscanf (blah, "%s %s %s", trash, akey, avalue);

  g_strdelimit (avalue, "|", ' ');

  sprintf (trash, _("Recorded alias \"%s\"\nAssociated action: %s\n"), akey,
	   avalue);

  textfield_add (trash, MESSAGE_NONE);

  g_free (trash);

  alias_insert (alias_table, akey, avalue);

  g_free (akey);
  g_free (avalue);
}

void
do_alias (char *blah)
{
  if (!strncmp (blah, "con", 3))
    connect_alias (blah);
  else if (!strncmp (blah, "disconnect", 3))
    disconnect ();
  else if (!strncmp (blah, "clear", 2))
    clear (0, GTK_TEXT_VIEW (mud->text));
  else if (!strncmp (blah, "help", 4))
    fake_help_thingy ();
  else if (!strncmp (blah, "qui", 3))
    bmud_exit ();
  else if (!strncmp (blah, "exi", 3))
    bmud_exit ();
  else if (!strncmp (blah, "cload", 5))
    color_load ();
  else if (!strncmp (blah, "csave", 5))
    color_save ();
  else if (!strncmp (blah, "cdef", 4))
    color_default ();
  else if (!strncmp (blah, "sload", 5))
    session_load ();
  else if (!strncmp (blah, "ssave", 5))
    session_save ();
  else if (!strncmp (blah, "sdef", 4))
    session_default ();
  else if (!strncmp (blah, "aload", 5))
    load_aliases (blah);
  else if (!strncmp (blah, "asave", 5))
    save_aliases (blah);
  else if (!strncmp (blah, "afree", 5))
    free_aliases ();
  else if (!strncmp (blah, "alias", 5))
    add_alias (blah);
  else
    alias_process (alias_table, blah);

  return;
}
