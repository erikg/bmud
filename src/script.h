#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#define SCRIPT_GUILE  0x01
#define SCRIPT_PERL   0x02
#define SCRIPT_PLUGIN 0x80	/* unused, reserved */

void script_init();
void script_load(char *, int);
void script_parse(char *);
void script_unload(int);
void script_shutdown();

#endif

