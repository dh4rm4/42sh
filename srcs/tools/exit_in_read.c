/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_in_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/28 12:09:26 by lfabbro           #+#    #+#             */
/*   Updated: 2017/04/28 12:09:26 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	exit_in_read(t_env *e)
{
	ft_prompt(e->prompt);
	ft_reset_line(e);
	return (0);
}
