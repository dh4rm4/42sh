/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcaps_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/30 11:41:22 by lfabbro           #+#    #+#             */
/*   Updated: 2017/02/09 17:31:40 by kboddez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		clear_cmd(t_env *e)
{
	tcaps_ctrl_end(e);
	xputs("dm");
	while (--TCAPS.nb_read + ((int)ft_strlen(e->prompt) + 1) > 0)
	{
		xputs("le");
		xputs("ce");
	}
	TCAPS.nb_read = 0;
	xputs("dl");
	xputs("ce");
	tputs(e->prompt, 1, dsh_putchar);
}

/*
 **	OPEN /tmp/.history AND STORE IT IN
 **	e->history TAB
 */


int		ft_read_history(t_env *e)
{
	int			fd;
	int 		i;
	char		**tmp;

	i = 0;
	tmp = NULL;
	if ((fd = open("/tmp/.history", O_RDONLY, OPENFLAGS)) == -1)
		return (ft_error(SH_NAME, "Cannot read /tmp/.history", NULL));
	if ((e->history = malloc(sizeof(*e->history) * 4096)) == NULL)
		return (ft_error(SH_NAME, "Malloc failed.", NULL));
	while (get_next_line(fd, &e->history[i]) > 0)
	{
		if (i >= 4096)
		{
			tmp = ft_tabnew(i * 2);
			ft_tabcpy(tmp, e->history);
			ft_tabfree(e->history);
			e->history = tmp;
		}
		++i;
	}
	e->history[i] = NULL;
	if (close(fd) == -1)
		return (ft_error(SH_NAME, "Close failed.", NULL));
	return (0);
}

/*
 **	ADD NEW CMD TO THE END OF HTE HISTORY TAB
 */

void	ft_check_history(t_env *e)
{
	int		i;
	int		accs;
	char	**tmp;

	accs = access("/tmp/.history", F_OK);
	ft_store_history(e->line);
	tmp = NULL;
	if (accs != -1)
	{
		i = ft_tablen(e->history);
		if (!ft_strcmp(e->history[i], e->line))
		{
			tmp = e->history;
			e->history = ft_tabcat(e->history, e->line);
			if (tmp)
				ft_free_tab(tmp);
		}
	}
	else if (e->history)
	{
		ft_free_tab(e->history);
		ft_read_history(e);
	}
	else
		ft_read_history(e);
}
/*
 **	MANAGE THE TERMCAPS HISTORY
 **	FOR UP ARROW
 */

void	tcaps_history_up(t_env *e)
{
	TCAPS.nb_move = TCAPS.nb_read;
	if (TCAPS.hist_move == -1)
		TCAPS.hist_move = (int)ft_tablen(e->history);
	if (e->history && e->history[0] && access("/tmp/.history", F_OK) != -1 &&
				TCAPS.hist_move >= 0)
	{
		clear_cmd(e);
		if (e->line)
			free(e->line);
		e->line = NULL;
		//	--TCAPS.hist_move;
		if (e->line)
			free(e->line);
		e->line = ft_strdup(e->history[TCAPS.hist_move]);
		ft_printf("%s", e->history[TCAPS.hist_move]);
		TCAPS.nb_read = (int)ft_strlen(e->history[TCAPS.hist_move]);
		TCAPS.nb_move = TCAPS.nb_read;
	}
	tcaps_recalc_pos(e);
//	ft_printf("%d\n", TCAPS.hist_move);
}

/*
 **	MANAGE THE TERMCAPS HISTORY
 **	FOR DOWN ARROW
 */

int		tcaps_history_down(t_env *e)
{
	int	tab_len;
	static int	last_nb_read = 0;

	TCAPS.nb_move = TCAPS.nb_read;
	tab_len = (int)ft_tablen(e->history);
	if (e->history && e->history[0])
	{
		if (TCAPS.hist_move == -1)
		{
			TCAPS.nb_read = last_nb_read;
			xputs("dm");
			while (--TCAPS.nb_read + ((int)ft_strlen(e->prompt) + 1) > 0)
			{
				xputs("le");
				xputs("ce");
			}
			TCAPS.nb_read = 0;
			xputs("dl");
			xputs("ce");
			tputs(e->prompt, 1, dsh_putchar);
			TCAPS.nb_read = 0;
			last_nb_read = 0;
			return (0);
			}
		clear_cmd(e);
		if (TCAPS.hist_move < tab_len)
		{
			TCAPS.nb_read = (int)ft_strlen(e->history[TCAPS.hist_move]);
			TCAPS.nb_move = TCAPS.nb_read;
			ft_printf("%s", e->history[TCAPS.hist_move]);
			if (e->line)
				free(e->line);
			e->line = ft_strdup(e->history[TCAPS.hist_move]);
//			++TCAPS.hist_move;
		}
	}
	if (TCAPS.hist_move == tab_len)
	{
		clear_cmd(e);
		xputs("cr");
		xputs("dm");
		while (--TCAPS.nb_read > 0)
		{
			xputs("le");
			xputs("dc");
		}
		xputs("dl");
		xputs("ce");
		xputs("ed");
		tputs(e->prompt, 1, dsh_putchar);
		tputs(e->line, 1, dsh_putchar);
//		TCAPS.hist_move = -1;
		last_nb_read = ft_strlen(e->line);
		if (e->line)
			free(e->line);
		e->line = NULL;
//		TCAPS.hist_move = -1;
	}
	tcaps_recalc_pos(e);
	return (0);
}
