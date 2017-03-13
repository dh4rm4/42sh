/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcaps_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/09 18:28:29 by lfabbro           #+#    #+#             */
/*   Updated: 2017/03/13 15:32:07 by kboddez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** RAZ LE BOL DE TOUT REECRIRE !
*/

void	xputs(char *tag)
{
	char	*res;

	res = tgetstr(tag, NULL);
	ft_putstr(res);
}

void	move_right(t_env *e)
{
	if (TCAPS.nb_col == (WIN_WIDTH - 1))
	{
		xputs("do");
		xputs("cr");
	}
	else
		xputs("nd");
	++TCAPS.nb_move;
	tcaps_recalc_pos(e);
}
