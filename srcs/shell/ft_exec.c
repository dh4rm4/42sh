#include "shell.h"

static int		fork_child(t_env *e, pid_t pid)
{
	t_job		*son;

	while (is_aggregator(e, RED_INDEX) || is_heredoc(e, RED_INDEX))
		struct_find_red(e);
	if (!(son = ft_new_job(e->jobs, pid)))
		return (ft_error(SH_NAME, "malloc failed", NULL));
	e->jobs = son;
	return (0);
}

static int		ft_fork_exec(char *exec, char **cmd, t_env *e)
{
	static int	prev_red_index = -1;
	pid_t		pid;

	if ((pid = fork()) < 0)
		ft_error(SH_NAME, "failed to fork process", NULL);
	if (pid)
	{
		++e->child_running;
		ft_close(FD.fd[1]);
		ft_close(FD.in);
	}
	else
	{
		if (redirection_before_cmd(e) == -1)
			exit(0);
		if ((is_input_in_next_cmd(e, RED_INDEX) || is_input_redir(e, RED_INDEX))
					&& RED_INDEX != prev_red_index)
			if (redir_input(e) == -1 && ft_printf("\n"))
				exit(0);
		ft_redirect(FD.in, STDIN_FILENO);
		execve(exec, &cmd[0], e->env);
	}
	prev_red_index = RED_INDEX;
	return (fork_child(e, pid));
}

int				ft_exec(char **cmd, t_env *e)
{
	int		ret;
	char	**paths;
	char	*exec;

	ret = 0;
	exec = NULL;
	paths = ft_find_paths(e->env);
	exec = ft_find_exec(paths, cmd[0]);
	if (!exec || access(exec, F_OK))
	{
		strfree(&exec);
		ft_free_tab(paths);
		paths = NULL;
		return (ft_error(cmd[0], "Command not found", NULL));
	}
	if (access(exec, X_OK | R_OK) == 0 || ft_issticky(exec))
		ret = ft_fork_exec(exec, cmd, e);
	else
		ret = ft_error(exec, "Permission denied", NULL);
	ft_free_tab(paths);
	paths = NULL;
	strfree(&exec);
	return (ret);
}

static int		exec_cmd_bis(t_env *e, t_logic *ptr)
{
	int		ret;
	int		stat;

	stat = 0;
	ret = 0;
	while (ptr)
	{
		if (ptr->op > 0)
			stat = ft_waitlogix(e);
		if (ptr->op < 0 || (ptr->op == AND && !ret && !stat) ||
				(ptr->op == OR && (ret || stat)))
		{
			if (ft_is_builtin(ptr->atom[0]))
				ret = ft_exec_builtin(e, ptr->atom);
			else
				ret = ft_exec(ptr->atom, e);
		}
		ptr = ptr->next;
	}
	return (ret);
}

int				ft_exec_cmd(t_env *e, char **cmd)
{
	int			ret;
	t_logic		*ptr;

	ret = 0;
	e->cmd_len = ft_tablen(cmd);
	tcaps_reset(e);
	if (e->cmd_len)
	{
		e->logix = ft_split_logic(e->logix, cmd);
		if (e->logix == NULL)
			return (ft_error(SH_NAME, "malloc failed.", NULL));
		ptr = e->logix;
		ret = exec_cmd_bis(e, ptr);
		ft_freelogic(e->logix);
	}
	e->cmd_len = 0;
	return (ret);
}
