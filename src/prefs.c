
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
 * $Id: prefs.c,v 1.5 2004/01/18 15:43:15 erik Exp $
 */

/* handles ~/.bmud/prefs */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "bmud.h"
#include "gui.h"
#include "main.h"
#include "misc.h"
#include "prefs.h"


FILE *fp;

void
session_scan ()
{
    char flag[1024], value[1024];

    if (fscanf (fp, "%s %s", flag, value) != 2)
	return;			/* wasn't real */
    if (!strncmp (flag, "hist_max", 8))
	mud->hist->max = atoi (value);
    else if (!strncmp (flag, "hist_cyclic", 10))
	mud->hist->cyclic = atoi (value);
    else if (!strncmp (flag, "font_name", 9))
    {
	if (mud->disp_font_name)
	    free (mud->disp_font_name);
	mud->disp_font_name = g_strdup (value);
    } else if (!strncmp (flag, "max_lines", 9))
	mud->maxlines = atoi (value);
    else if (!strncmp (flag, "stat_size", 9))
	mud->statsize = atoi (value);
    return;
}

int
session_load ()
{
    int x;

    mud->hist->size = 0;
    mud->hist->cur = 0;
    mud->hist->pos = 0;
    mud->lines = 0;

    if (show_conf_dir ("") == NULL)
    {
	session_default ();
	return 1;
    }

    if ((fp = fopen (show_conf_dir ("prefs"), "r")) == NULL)
    {
	session_default ();
	return 1;
    }
    while (!feof (fp))
	session_scan ();
    fclose (fp);

    if (mud->hist->list)
	free (mud->hist->list);
    mud->hist->list = g_malloc (sizeof (void *) * mud->hist->max);

    for (x = 0; x < mud->hist->max; x++)
	mud->hist->list[x] = NULL;

    return 1;
}

void
session_default ()
{
    int x;

/*
  textfield_add ("Could not open prefs file, using defaults\n",
		 MESSAGE_ERR);
*/
    printf ("Could not open prefs file, using defaults\n");

    mud->hist->max = 20;
    mud->hist->cyclic = 0;
    mud->disp_font_name = "courier";
    mud->maxlines = 200 * 60;	/* roughly 60 bytes per line? */
    mud->statsize = 150;

    mud->hist->list = g_malloc (sizeof (void *) * mud->hist->max);

    for (x = 0; x < mud->hist->max; x++)
	mud->hist->list[x] = NULL;

    return;
}

int
session_save ()
{
    textfield_add ("Saving session\n", MESSAGE_ERR);
    printf ("Saving session\n");

/*		havoc said this way is better, but I can't make it work :/	
	fp = (FILE *)open(show_conf_dir("prefs"),O_CREAT | O_EXCL | O_TRUNC | O_RDWR, 0600);
	if( fp == NULL )
*/
    if ((fp = fopen (show_conf_dir ("prefs"), "w")) == NULL)
    {
	textfield_add ("Unabled to open prefs file for writing\n",
	    MESSAGE_ERR);
	printf ("Unabled to open prefs file for writing\n");
	return -1;
    }

    fprintf (fp, "hist_max %d\n", mud->hist->max);
    fprintf (fp, "hist_cyclic %d\n", mud->hist->cyclic);
    fprintf (fp, "font_name %s\n", mud->disp_font_name);
    fprintf (fp, "max_lines %d\n", mud->maxlines);
    fprintf (fp, "stat_size %d\n", mud->statsize);

/*		window position stuff someday?
	sprintf(tmp,"%dx%d",mud->window
	fprintf(fp,"window_position %s",tmp);
	sprintf(tmp,"%dx%d",mud->window-
*/

    fclose (fp);

    return 0;
}
