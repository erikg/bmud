
/*****************************************************************************
 *    BMUD - Br0kEs MUD Client                                               *
 *                                                                           *
 *     Copyright (C) 1998-2005 Erik Greenwald <erik@smluc.org>               *
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
 * $Id: color.c,v 1.21 2008/06/16 10:02:13 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#ifdef USE_GNOME
#ifndef GNOME_H
#include <gnome.h>
#endif
#else
#include <gtk/gtk.h>
#endif

#include "bmud.h"
#include "color.h"
#include "gui.h"
#include "main.h"
#include "misc.h"

GtkWidget *cs, *cm;
static FILE *fp;

GtkWidget *text_field;
GtkWidget *text_entry;
GtkWidget *entry_host;
GtkWidget *entry_port;

GdkColormap *cmap;

GdkColor color[9][2];

gchar *cn[9][2] = {
    {"Black", "Grey"}
    ,
    {"Red", "Pink"}
    ,
    {"Green", "Light_Green"}
    ,
    {"Orange", "Yellow"}
    ,
    {"Blue", "Light_Blue"}
    ,
    {"Maroon", "Purple"}
    ,
    {"Cyan", "Light_Cyan"}
    ,
    {"Silver", "White"}
    ,
    {"MESSAGE", "ERROR"}
    ,
};

/*** Escape is detected, see which color it is ***/

/* this could ***REALLY*** use some work :/ kinda a quicky hack */

/* 'else' isn't used because return terminates the progression */
GdkColor
getcol (char *tmp, int bleh)
{
    switch (bleh)
    {
    case 2:
	if (!strncmp (tmp, "[m", 2))
	    return color[7][0];
	break;
    case 3:
	if (!strncmp (tmp, "[1m", 3))
	    return color[7][0];
	break;
    case 4:
	if (!strncmp (tmp, "[30m", 4))
	    return color[0][0];
	if (!strncmp (tmp, "[31m", 4))
	    return color[1][0];
	if (!strncmp (tmp, "[32m", 4))
	    return color[2][0];
	if (!strncmp (tmp, "[33m", 4))
	    return color[3][0];
	if (!strncmp (tmp, "[34m", 4))
	    return color[4][0];
	if (!strncmp (tmp, "[35m", 4))
	    return color[5][0];
	if (!strncmp (tmp, "[36m", 4))
	    return color[6][0];
	if (!strncmp (tmp, "[37m", 4))
	    return color[7][0];
	break;
    case 6:
	if (!strncmp (tmp, "[0;30m", 6))
	    return color[0][0];
	if (!strncmp (tmp, "[0;31m", 6))
	    return color[1][0];
	if (!strncmp (tmp, "[0;32m", 6))
	    return color[2][0];
	if (!strncmp (tmp, "[0;33m", 6))
	    return color[3][0];
	if (!strncmp (tmp, "[0;34m", 6))
	    return color[4][0];
	if (!strncmp (tmp, "[0;35m", 6))
	    return color[5][0];
	if (!strncmp (tmp, "[0;36m", 6))
	    return color[6][0];
	if (!strncmp (tmp, "[0;37m", 6))
	    return color[7][0];
	if (!strncmp (tmp, "[1;30m", 6))
	    return color[0][1];
	if (!strncmp (tmp, "[1;31m", 6))
	    return color[1][1];
	if (!strncmp (tmp, "[1;32m", 6))
	    return color[2][1];
	if (!strncmp (tmp, "[1;33m", 6))
	    return color[3][1];
	if (!strncmp (tmp, "[1;34m", 6))
	    return color[4][1];
	if (!strncmp (tmp, "[1;35m", 6))
	    return color[5][1];
	if (!strncmp (tmp, "[1;36m", 6))
	    return color[6][1];
	if (!strncmp (tmp, "[1;37m", 6))
	    return color[7][1];
	if (!strncmp (tmp, "[J", 2))
	    return color[7][0];
	break;
    case 8:
	if (!strncmp (tmp, "[0;1;30m", 8))
	    return color[0][1];
	if (!strncmp (tmp, "[0;1;31m", 8))
	    return color[1][1];
	if (!strncmp (tmp, "[0;1;32m", 8))
	    return color[2][1];
	if (!strncmp (tmp, "[0;1;33m", 8))
	    return color[3][1];
	if (!strncmp (tmp, "[0;1;34m", 8))
	    return color[4][1];
	if (!strncmp (tmp, "[0;1;35m", 8))
	    return color[5][1];
	if (!strncmp (tmp, "[0;1;36m", 8))
	    return color[6][1];
	if (!strncmp (tmp, "[0;1;37m", 8))
	    return color[7][1];
	if (!strncmp (tmp, "[0;0;30m", 8))
	    return color[0][0];
	if (!strncmp (tmp, "[0;0;31m", 8))
	    return color[1][0];
	if (!strncmp (tmp, "[0;0;32m", 8))
	    return color[2][0];
	if (!strncmp (tmp, "[0;0;33m", 8))
	    return color[3][0];
	if (!strncmp (tmp, "[0;0;34m", 8))
	    return color[4][0];
	if (!strncmp (tmp, "[0;0;35m", 8))
	    return color[5][0];
	if (!strncmp (tmp, "[0;0;36m", 8))
	    return color[6][0];
	if (!strncmp (tmp, "[0;0;37m", 8))
	    return color[7][0];
	if (!strncmp (tmp, "[0;5;30m", 8))
	    return color[0][1];
	if (!strncmp (tmp, "[0;5;31m", 8))
	    return color[1][1];
	if (!strncmp (tmp, "[0;5;32m", 8))
	    return color[2][1];
	if (!strncmp (tmp, "[0;5;33m", 8))
	    return color[3][1];
	if (!strncmp (tmp, "[0;5;34m", 8))
	    return color[4][1];
	if (!strncmp (tmp, "[0;5;35m", 8))
	    return color[5][1];
	if (!strncmp (tmp, "[0;5;36m", 8))
	    return color[6][1];
	if (!strncmp (tmp, "[0;5;37m", 8))
	    return color[7][1];
	break;
    default:
	return color[7][0];
    }
    return color[7][0];
}

/*** Scan thru the text to look for escapes ***/
color_tag_t *
disp_ansi (char *dst, char *src)
{
    /* GdkColor col = color[7][0]; */
    int n = 0, x = 0, j = 0, size;

    size = strlen (src);

    while (n < size)
    {
	/*
	 * strip CR from CRLF or LFCR as per rfc 854 :/ 
	 */
	if (src[n] == 13)
	    ++n;

	/*
	 * remembers how many lines we have src 
	 */
	/*
	 * else if(src[n]=='\n')
	 * {
	 * mud->lines++;
	 * if(mud->lines>200)
	 * cleartail();
	 * }
	 */
	/*
	 * this is for the password block 
	 */
	if (src[n] == -1 && src[n + 2] == 1)
	{
	    if (src[n + 1] == -5)
		GTK_ENTRY (mud->ent)->visible = 0;
	    if (src[n + 1] == -4)
		GTK_ENTRY (mud->ent)->visible = 1;
	    n += 3;
	}

	/*
	 * if it's not a 'special' code, prsrct 
	 */
	else if (src[n] != 27)
	    dst[j++] = src[n];

	/*
	 * this is an ansi code, stripping time 
	 */
	else
	{
	    while (!isalpha (src[x + n]))
		++x;
	    /*
	     * if (x < 9)
	     * col = getcol (&src[n + 1], x);
	     */
	    n += x;
	    x = 0;
	}
	++n;
    }

    return NULL;
}

void
set_color (GdkColor * colorx, int r, int g, int b)
{
    colorx->red = (r * 256) % 65535;	/* is mod needed at all? */
    colorx->green = (g * 256) % 65535;
    colorx->blue = (b * 256) % 65535;
    if (!gdk_color_alloc (cmap, colorx))
	g_error ("Color allocation failed\n");
    return;
}

void
color_scan ()
{
    char colorx[1024], r[1024], b[1024], g[1024];
    int x, y;

    if (fscanf (fp, "%s %s %s %s", colorx, r, g, b) != 4)
	return;			/* wasn't real */
    for (x = 0; x < 9; x++)
	for (y = 0; y < 2; y++)
	    if (!strcmp (colorx, cn[x][y]))
		set_color (&color[x][y], atoi (r), atoi (g), atoi (b));
    return;
}

int
color_load ()
{
    cmap = gdk_colormap_get_system ();
    if (show_conf_dir ("") == NULL)
    {
	color_default ();
	return 1;
    }
    if (!(fp = fopen (show_conf_dir ("colors"), "r")))
    {
	color_default ();
	return 1;
    }
    while (!feof (fp))
	color_scan ();
    fclose (fp);
    return 1;
}

int
color_save ()
{
    int x, y;

    check_my_dir ();		/* in misc.c, creates a dir if not existing */
    /*
     * if((fp=(FILE *)open(show_conf_dir("colors"),O_CREAT | O_EXCL | O_TRUNC | O_RDWR, 0600))==-1)
     */
    if (!(fp = fopen (show_conf_dir ("colors"), "w")))
    {
	textfield_add ("Unabled to open color file for writing\n",
	    MESSAGE_ERR);
	printf ("Unabled to open color file for writing\n");
	printf ("%s\n", show_conf_dir ("colors"));
	return -1;
    }
    for (x = 0; x < 9; x++)
	for (y = 0; y < 2; y++)
	    fprintf (fp, "%s %d %d %d\n", cn[x][y], color[x][y].red / 256,
		color[x][y].green / 256, color[x][y].blue / 256);
    fclose (fp);
    return 0;
}

int
color_default ()
{

/*
  textfield_add ("Couldn't open color file, using defaults\n",
		 MESSAGE_ERR);
*/
    printf ("Couldn't open color file, using defaults\n");

    set_color (&color[0][0], 0, 0, 0);
    set_color (&color[0][1], 190, 190, 190);
    set_color (&color[1][0], 255, 0, 0);
    set_color (&color[1][1], 255, 192, 203);
    set_color (&color[2][0], 0, 211, 0);
    set_color (&color[2][1], 0, 255, 0);
    set_color (&color[3][0], 255, 165, 0);
    set_color (&color[3][1], 255, 255, 0);
    set_color (&color[4][0], 0, 100, 255);
    set_color (&color[4][1], 173, 216, 230);
    set_color (&color[5][0], 165, 42, 42);
    set_color (&color[5][1], 160, 32, 240);
    set_color (&color[6][0], 0, 255, 255);
    set_color (&color[6][1], 224, 255, 255);
    set_color (&color[7][0], 211, 211, 211);
    set_color (&color[7][1], 255, 255, 255);
    set_color (&color[8][0], 255, 255, 255);
    set_color (&color[8][1], 255, 0, 0);
    return 0;
}
