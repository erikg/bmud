
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
 * $Id: net.c,v 1.10 2008/06/16 10:16:06 erik Exp $
 */

/* 
   This version of net.c has been hacked and rewritten by Robert J. Hansen
   (rjhansen@inav.net) in the hopes of making it easier to read, understand
   and (maybe even) re-use.

   Deprecated calls, like bzero and the like, have been converted to
   memset()s.

   Original author (not RJH) says that most of this code came straight
   from amcl, whatever that is.  Whatever amcl is, it sure used an awful
   lot of deprecated bits.

     * amcl is a mud that released version 0.1 2 days before bmud 0.1 came out.
     * I looked at it and said "I can do better" and started bmud, so at first
     * I stole all the networking code and color code and put my own UI around
     * it. Looking back, it was probably a stupid move. Concentrating effort on
     * on project would have been a better move than essentially forking. Ah 
     * well, life happens.
     * This section needs a full re-write anyways, hopefully threaded. Should
     * be made ready for multi-connect usage. I also want freebsd friendly.
     * I'll go ahead and slap this in 
     * for 0.5 anyways.
     * -Erik <erik@smluc.org>
*/

#include <sys/types.h>
#include <netinet/in.h>		/* breaks bsd */
#include <sys/socket.h>		/* breaks bsd */
#include <gtk/gtk.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/telnet.h>

#include "alias.h"
#include "bmud.h"
#include "gui.h"
#include "main.h"
#include "misc.h"
#include "net.h"

/*
   Global Variables.  Icky poo.  Can we find some more aesthetically
   pleasing solution?

   Instead of having lots of globals, we've now got a single global
   struct which contains all of the previous globals.  It doesn't
   change anything, except that it strikes me as being more aesthetically
   pleasing.  -- RJH
*/

/* this may become an array of globals. -Erik */

struct one_global {
    int sockfd, connected, echo;
    gint input_monitor;
} global;

void
make_connection (char *host, char *port)
{
    /*
     * In one of the header files BUFSIZ is declared.  Figured
     * we might as well use it instead of the 2,048-byte 
     * buffer.  On x86 systems BUFSIZ = 8192.
     * -- RJH
     */

    char buf[BUFSIZ];
    memset (buf, 0, BUFSIZ * sizeof (char));

    if (strcmp (host, "\0") == 0)
    {
	sprintf (buf, "\n*** Can't connect - you didn't specify a host\n");
	textfield_add (buf, MESSAGE_ERR);
	return;
    }

    if (strcmp (port, "\0") == 0)
    {
	sprintf (buf, "\n*** No port specified - assuming port 23\n");
	textfield_add (buf, MESSAGE_NORMAL);

	/*
	 * Original code had 'port = "23\0"'.  The trailing \0 was
	 * redundant: anything in quotes is null-terminated by C.
	 * -- RJH
	 */

	port = "23";
    }

    sprintf (buf, "\n*** Making connection to %s, port %s\n", host, port);
    textfield_add (buf, MESSAGE_NORMAL);

    open_connection (host, port);
}

void
disconnect (void)
{
    close (global.sockfd);

    if (global.input_monitor)	/* else a gdk assert fails -Erik jul03-1999 */
	gdk_input_remove (global.input_monitor);

    textfield_add ("\n*** Connection closed.\n", MESSAGE_NORMAL);
    global.connected = FALSE;
}

void
open_connection (const char *host, const char *port)
{
    char buf[BUFSIZ];
    int N;
    struct hostent *he = gethostbyname (host);
    struct sockaddr_in their_addr;

    memset (buf, 0, BUFSIZ * sizeof (char));

    if (he == NULL)
    {
	textfield_add ("\n*** gethostbyname() failed! ***\n", MESSAGE_ERR);
	return;
    }

    global.sockfd = socket (AF_INET, SOCK_STREAM, 0);

    if (global.sockfd == -1)
    {
	textfield_add (strerror (errno), MESSAGE_ERR);
	return;
    }

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons (atoi (port));
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);

    memset ((void *)(&their_addr.sin_zero), 0, 8);

    N = connect (global.sockfd, (struct sockaddr *)&their_addr,
	sizeof (struct sockaddr));

    if (N == -1)
    {
	textfield_add (strerror (errno), MESSAGE_ERR);
	return;
    }

    textfield_add ("\n*** Connection established.\n", MESSAGE_NORMAL);

    global.input_monitor =
	gdk_input_add (global.sockfd, GDK_INPUT_READ, read_from_connection,
	NULL);

    global.connected = TRUE;

}

void
read_from_connection (gpointer data, gint source, GdkInputCondition condition)
{
    /*
     * For some reason there's a BUFSIZ defined in the header files.
     * Figured we might as well use that instead of the original 
     * 2,048-byte buffer.  On Intel x86 systems, BUFSIZ = 8192.
     * -- RJH
     */

    char buf[BUFSIZ];
    int numbytes;

    /*
     * Found a bug here.  The following line is incorrect:
     * 
     * memset(buf, 0, sizeof(char));
     * 
     * -- RJH
     */

    data = NULL;
    source = 0;
    condition = 0;
    memset (buf, 0, BUFSIZ * sizeof (char));

    /*
     * Originally this code used a recv() call, but recv() and
     * recvfrom() have a lot of overlap and the manpages say
     * one or the other is going to go bye-bye.  Hence, I've
     * changed this to be a read() call, which should be around
     * for a long time.  :)   -- RJH
     */

    numbytes = read (global.sockfd, buf, BUFSIZ);

    if (numbytes == -1)
    {
	textfield_add (strerror (errno), MESSAGE_ERR);
	disconnect ();
	return;
    }

    /*
     * This statement is extraneous: since the entire buffer has been
     * memset()ted to 0, there's no need to null-terminate what we
     * read into the buffer.  -- RJH
     * 
     * buf[numbytes] = '\0';
     */

    /*
     * Sometimes we get here even though there isn't any data to read
     * from the socket..
     *
     * found by Michael Stevens
     */
    if (numbytes == 0)
    {
	textfield_add ("\n*** 0 byte packet received\n", MESSAGE_ERR);
	disconnect ();
	return;
    }

    /*
     * And why in the world is this here?  It's redundant.  :)
     * -- RJH
     * 
     * buf[numbytes]=0;
     */

    textfield_add (buf, MESSAGE_ANSI);
    return;
}

void
xmit_to_connection (gchar * text)
{
    write (global.sockfd, text, strlen (text));
    write (global.sockfd, "\r\n", 2);

    if (global.echo)
    {
	textfield_add (text, MESSAGE_ANSI);
	textfield_add ("\n", MESSAGE_NONE);
    }
}

void
send_to_connection (GtkWidget * widget, gpointer data)
{
    gchar *entry_text;

    widget = NULL;
    data = NULL;

    entry_text = (gchar *) gtk_entry_get_text (GTK_ENTRY (mud->ent));
    gtk_entry_select_region (GTK_ENTRY (mud->ent), 0,
	GTK_ENTRY (mud->ent)->text_length);

    hist_add (entry_text);	/* add it to the command history */

    if (entry_text[0] == '/' || entry_text[0] == '#')
    {
	do_alias ((char *)&entry_text[1]);	/* strips the / or # */
	gtk_entry_set_text (GTK_ENTRY (mud->ent), "");
	return;
    }

    /*
     * Originally these write() calls were send() calls.  Send()
     * isn't in danger of being deprecated, but for symmetry with
     * the read() calls I decided to do it this way anyway.
     * 
     * Also, the "send(sockfd, "\n", 1, 0)" should have been
     * "send(sockfd, "\r\n", 2, 0).  \r\n is a two-byte sequence,
     * not a one-fer.  :)
     * -- RJH
     */

    /*
     * the write/send crap moved to "xmit_to_connection" to help facilitate
     * * aliases. Nov 24 1999 Erik 
     */

    xmit_to_connection (entry_text);

    gtk_entry_set_text (GTK_ENTRY (mud->ent), "");	/* clear the entry line */
    gtk_widget_grab_focus (GTK_WIDGET (mud->ent));

    textfield_add ("\n", MESSAGE_NONE);
}
