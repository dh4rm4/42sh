/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/21 18:55:15 by lfabbro           #+#    #+#             */
/*   Updated: 2017/02/02 18:23:48 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char		**ft_parse_cmd(t_env *e)
{
	char	**cmds;
	char	*trline;

	trline = ft_strxtrim_quote(e->line, '\t');
	cmds = ft_strsplit(trline, ';');
	free(trline);
	return (cmds);
}

static int		ft_exec_builtin(t_env *e)
{
	char	ret;

	ret = 0;
//	store_history(e->cmd);
	if (ft_strequ(e->cmd[0], "exit") && ++ret)
		ft_exit(e);
	else if (ft_strequ(e->cmd[0], "env") && ++ret)
		ft_env(e);
	else if (ft_strequ(e->cmd[0], "setenv") && ++ret)
		ft_setenv_blt(e);
	else if (ft_strequ(e->cmd[0], "unsetenv") && ++ret)
		ft_unsetenv_blt(e);
	else if (ft_strequ(e->cmd[0], "cd") && ++ret)
		ft_chdir(e);
	else if (ft_strequ(e->cmd[0], "echo") && ++ret)
		ft_echo(e);
	else if (ft_strequ(e->cmd[0], "where") && ++ret)
		ft_where(e);
	else if (ft_strequ(e->cmd[0], "history") && ++ret)
		ft_history(e);
	return (ret);
}

int				ft_exec_cmd(t_env *e, char *cmd, int in, int fd[2])
{
	int		ret;

	ret = 0;
	e->cmd = ft_strsplit_quote(cmd, ' ');
	e->cmd_len = ft_tablen(e->cmd);
	if (e->cmd_len)
	{
		if ((ret = ft_exec_builtin(e)))
			;
		else
			ret = ft_exec(e->cmd, e->env, in, fd);
	}
	ft_check_history(e);
	ft_free_tab(e->cmd);
	e->cmd = NULL;
	e->cmd_len = 0;
	return (ret);
}

int				ft_split_pipes(t_env *e, char *cmds_i)
{
	int		i;
	int		in;
	int		fd[2];
	int		ret;
	char	**pipes;

	i = -1;
	in = STDIN_FILENO;
	pipes = ft_strsplit_quote(cmds_i, '|');
	while (pipes[++i + 1])
	{
		ft_printf("pipe[%d]: %s\n", i, pipes[i]);
		if (pipe(fd) < 0)
		{
			ft_free_tab(pipes);
			return (ft_error(SH_NAME, "Pipe failed.", NULL));
		}
		ret = ft_exec_cmd(e, pipes[i], in, fd);
		in = fd[0];
	}
	ft_printf("pipe[%d]: %s\n", i, pipes[i]);
	fd[1] = STDOUT_FILENO;
	ret = ft_exec_cmd(e, pipes[i], in, fd);
	ft_free_tab(pipes);
	return (ret);
}

int				ft_parse_line(t_env *e)
{
	int		i;
	int		ret;
	char	**cmds;

	i = -1;
	ret = 0;
	if ((cmds = ft_parse_cmd(e)) != NULL)
	{
		while (cmds[++i])
		{
			//ft_redirect(&cmds[i], e);
			if (ft_matchquotes(cmds[i]) == 0)
			{
				ret = ft_split_pipes(e, cmds[i]);
			}
			else
				ft_error(NULL, "Unmatched quote", NULL);
		}
	}
	ft_free_tab(cmds);
	return (ret);
}
