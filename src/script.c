#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define _XOPEN_SOURCE
#define __XOPEN_SOURCE

#ifdef USE_GUILE
#include <guile/gh.h>
#endif

#ifdef USE_PERL
#undef PACKAGE
#include <EXTERN.h>
#include <perl.h>
#undef _
static PerlInterpreter *perl_env;
#endif


#include "bmud.h"
#include "gui.h"
#include "misc.h"
#include "script.h"

	/* such a nasty kludge. */
void
fake_main(int argc, char **argv)
{
	char *filename;
	filename = gethome_conf_file("script.scm");
	script_load(filename, SCRIPT_GUILE);
	gtk_main();
}

void
script_init (int argc, char **argv)
{
#ifdef USE_PERL
	char *filename;
	perl_env = perl_alloc();
	perl_construct(perl_env);
	filename = gethome_conf_file("script.pl");
	script_load(filename, SCRIPT_PERL);
#endif

#ifdef USE_GUILE
	gh_enter(argc, argv, fake_main);
#endif
}

void
script_load (char *filename, int type)
{
	int fh;
	char *buf, *tmpbuf;
	struct stat s;
	
	if( ( fh = open(filename, O_RDONLY) ) == -1)
	{
		perror("BMUD script");
		textfield_add(strerror(errno), MESSAGE_ERR);
		textfield_add(": ", MESSAGE_ERR);
		textfield_add(filename, MESSAGE_ERR);
		textfield_add("\n", MESSAGE_ERR);
		return;
	}
	fstat( fh, &s );
	buf = (char *)malloc( s.st_size );
	read ( fh, buf, s.st_size);
	close(fh);
#ifdef USE_GUILE
	if(type == SCRIPT_GUILE)
	{
		gh_eval_str(buf);
		free(buf);
		return;
	}
#endif
#ifdef USE_PERL
	perl_parse(perl_env, NULL, 1, NULL, NULL);
#endif	
	tmpbuf=(char *)malloc(1024);
	sprintf(tmpbuf, _("I can't support %s.\n"), filename);
	textfield_add(tmpbuf, MESSAGE_ERR);
	free(buf);
	free(tmpbuf);
	return;
}

void
script_parse (char *string)
{
#ifdef USE_GUILE
	gh_eval_str(string);
#endif
#ifdef USE_PERL
/*	perl_method_call();	*/
#endif
}

void
script_unload (int type)
{
#ifdef USE_GUILE
#endif
}

void
script_shutdown ()
{
#ifdef USE_GUILE
#endif

#ifdef USE_PERL
	perl_destruct(perl_env);
	perl_free(perl_env);
#endif
}

