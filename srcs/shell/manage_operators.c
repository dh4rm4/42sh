#include "shell.h"

static int	find_next_op(t_env *e)
{
	int		i;

	i = 0;
	while (e->magic[i].cmd && !is_operator(e, i))
		++i;
	if (!e->magic[i].cmd)
		return (0);
	return (i);
}

int			manage_operators(t_env *e, int i, int ret)
{
	static	int		next_op = 0;

	if (!i)
		next_op = 0;
	if (!(next_op = find_next_op(e)))
		return (0);
	e->check_input = 0;
	if (is_and(e, next_op) && ret == 1)
		return (0);
	else if (is_and(e, next_op) && ret == -1)
		return (find_nxt_operator(e));
	else if (is_or(e, next_op) && ret == 1)
		return (find_nxt_operator(e));
	else if (is_or(e, next_op) && ret == -1)
		return (0);
	return (0);
}