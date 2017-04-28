/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/28 12:07:13 by lfabbro           #+#    #+#             */
/*   Updated: 2017/04/28 20:12:51 by vlistrat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int		add_job(t_env *e, pid_t pid)
{
	t_job		*son;

	if (!(son = ft_new_job(e->jobs, pid)))
		return (ft_error(NULL, "malloc failed", NULL));
	e->jobs = son;
	return (1);
}

int				ft_fork_exec(char *exec, char **cmd, t_env *e)
{
	pid_t		pid;

	if ((redirection_before_cmd(e)) < 0)
	{
		dup2(FD.stdout, STDOUT_FILENO);
		dup2(FD.stdin, STDIN_FILENO);
		dup2(FD.stderr, STDERR_FILENO);
		ft_close(FD.fd[1]);
		return (1);
	}
	if ((RED_INDEX != e->len_mag || is_next_redir(e, RED_INDEX) != -1)
		&& !e->is_out_close)
		ft_close(FD.fd[1]);
	if ((pid = fork()) < 0)
		ft_error(NULL, "failed to fork process", NULL);
	if (!pid)
	{
		ft_redirect(FD.in, STDIN_FILENO);
		execve(exec, &cmd[0], e->env);
	}
	++e->child_running;
	dup2(FD.stdout, STDOUT_FILENO);
	dup2(FD.stdin, STDIN_FILENO);
	dup2(FD.stderr, STDERR_FILENO);
	return (add_job(e, pid));
}

static int		ft_exec(char **cmd, t_env *e, char *exec, int ret)
{
	char	**paths;

	paths = ft_find_paths(e->env);
	if (!(exec = ft_find_exec(paths, cmd[0])))
	{
		if (paths)
			ft_free_tab(paths);
		return (print_command_not_found(cmd[0], e));
	}
	if (!exec || access(exec, F_OK))
	{
		strfree(&exec);
		ft_free_tab(paths);
		paths = NULL;
		redirection_before_cmd(e);
		return (print_command_not_found(cmd[0], e));
	}
	if (paths)
		ft_free_tab(paths);
	paths = NULL;
	return (ft_exec_bis(cmd, e, exec, ret));
}

static int		exec_cmd_bis(t_env *e, char **cmd)
{
	int		ret;

	ret = 0;
	if (ft_is_builtin(cmd[0]) && !e->env_exec)
	{
		ret = ft_exec_builtin(e, cmd, 0);
		e->last_cmd_ret = (ret > 0) ? 0 : 1;
	}
	else
		ret = ft_exec(cmd, e, NULL, 0);
	return (ret);
}

int				ft_exec_cmd(t_env *e, char **cmd)
{
	int			ret;

	ret = 0;
	e->cmd_len = ft_tablen(cmd);
	tcaps_reset(e);
	if (e->cmd_len)
		ret = exec_cmd_bis(e, cmd);
	else
		return (-1);
	e->cmd_len = 0;
	return (ret);
}
