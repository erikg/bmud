
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
 * $Id: color.h,v 1.7 2004/01/19 18:21:49 erik Exp $
 */

typedef struct color_tag_s {
    GtkTextTag tag;
    GtkTextIter start, end;
    struct color_tag_s *next;
} color_tag_t;

extern GdkColor *foreground;
extern GdkColor color[9][2];
extern gchar *cn[9][2];		/* names of colors */

extern void show_colorm (void);
extern void init_colors ();
extern void disp_ansi (char *dst, char *src, color_tag_t ** tags);
extern int color_load ();
extern int color_default ();
extern int color_dump ();
extern int color_save ();
