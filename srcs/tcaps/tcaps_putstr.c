#include "shell.h"

/*
**  sc: save position curosr
**  cr: carriage return (debut de ligne)
**  dm: start delete mode
**  le: left
**  dc: delete one char
**  dl: delete line
**  ce: delete line from cursor
**  ed: end delete mode
**  rc: recover cursor position
*/

int     tcaps_putstr(t_env *e, char *str)
{
	int     l;

	xputs("sc");
	xputs("cr");
	xputs("dm");
	l = TCAPS.nb_read;
	while (--l > 0)
	{
		xputs("le");
		xputs("dc");
	}
	xputs("dl");
	xputs("ce");
	xputs("ed");
	tcaps_prompt(e->prompt);
	tputs(str, 1, dsh_putchar);
	xputs("rc");
	return (0);
}

