/* this file came from Erik Thyren, I moved it out of gui.c because it's
 * fairly large and fairly specific */

#include <stdio.h>
#include <stdlib.h>
#include "bmud.h"
#include "gui.h"
#include "main.h"
#include "misc.h"
#include "net.h"

struct server
{
  char *host;
  char *port;
  struct server *last;
};

struct server *last;		/* to prevent a memory leak, a bass ackwards linked list
				   is built. This is the final node of that list. */

void
read_file_connect (GtkButton * button, struct server *this)
{
	button = NULL;
  make_connection (this->host, this->port);
  destructify ();

  while (last->last != NULL)	/* clean the linked list by traversing from */
    {				/* last node. */
      this = last;
      last = this->last;
      free (this);
    }

  return;
}

void
clist ()
{
  FILE *fp;
  char *host, *port;
  struct server *this;
  GtkWidget *vbox, *label, *button, *separator;

  host = g_malloc (sizeof (char) * 65535);
  port = g_malloc (sizeof (char) * 65535);

  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_border_width (GTK_CONTAINER (win), 10);
  vbox = gtk_vbox_new (0, 5);

  label = gtk_label_new (_("BMUD Serverlist"));
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, FALSE, 0);
  gtk_widget_show (label);
  gtk_widget_show (separator);

  fp = fopen (show_conf_dir ("mudservers"), "r");
  if (fp != NULL)
    {
      while ((fscanf (fp, "%s %s", host, port)) == 2)
	{
	  struct server *last;
	  last = this;
	  this = (struct server *) malloc (sizeof (void *) * 2);
	  this->host = g_strdup (host);
	  this->port = g_strdup (port);
	  this->last = last;
	  button = gtk_button_new_with_label (host);
	  gtk_signal_connect (GTK_OBJECT (button), "clicked",
			      GTK_SIGNAL_FUNC (read_file_connect), this);
	  gtk_widget_show (button);
	  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
	}
      fclose (fp);

      last = this;

      button = gtk_button_new_with_label (_("Cancel"));
      gtk_signal_connect (GTK_OBJECT (button), "clicked",
			  GTK_SIGNAL_FUNC (destructify), NULL);
      gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, FALSE, 0);
      gtk_widget_show (button);
    }
  else
    {
      label = gtk_label_new (_("Sorry, Couldn't open serverlist"));
      button = gtk_button_new_with_label ("Ok");
      gtk_signal_connect (GTK_OBJECT (button), "clicked",
			  GTK_SIGNAL_FUNC (destructify), NULL);
      gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
      gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, TRUE, 0);
      gtk_widget_show (label);
      gtk_widget_show (button);
    }

  gtk_container_add (GTK_CONTAINER (win), vbox);
  gtk_widget_show (vbox);
  gtk_widget_show (win);

  return;
}
