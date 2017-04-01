#include "shell.h"

static int		fork_child(t_env *e, pid_t pid)
{
	t_job		*son;

	if (!(son = ft_new_job(e->jobs, pid)))
		return (ft_error(SH_NAME, "malloc failed", NULL));
	e->jobs = son;
	return (1);
}

static int		ft_fork_exec(char *exec, char **cmd, t_env *e)
{
	pid_t		pid;

	redirection_before_cmd(e);
	ft_close(FD.fd[1]);
	if ((pid = fork()) < 0)
		ft_error(SH_NAME, "failed to fork process", NULL);
	if (pid)
	{
		++e->child_running;
//		ft_close(FD.in);
	}
	else
	{
		ft_redirect(FD.in, STDIN_FILENO);
		execve(exec, &cmd[0], e->env);
	}
	dup2(FD.stdout, STDOUT_FILENO);
	dup2(FD.stdin, STDIN_FILENO);
	dup2(FD.stderr, STDERR_FILENO);
	return (fork_child(e, pid));
}


int				print_command_not_found(char *cmd, t_env *e)
{
	char *number;
	char *string;

	if (e->raw)
	{
		number = ft_itoa(-e->raw);
		string = ft_strjoin("line ", number);
		ft_error(cmd, string , "Command not found");
		ft_strdel(&number);
		ft_strdel(&string);
	}
	else
		ft_error(cmd, "Command not found", NULL);
	return (-1);
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
		return (print_command_not_found(cmd[0], e));
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

static int		exec_cmd_bis(t_env *e, char **cmd)
{
	int		ret;

	ret = 0;
	if (ft_is_builtin(cmd[0]))
	{
		ret = ft_exec_builtin(e, cmd);
		reset_last_ret(e, ((ret == 1) ? 0 : 127));

	}
	else
	{
		ft_exec(cmd, e);
		ret = ft_waitlogix(e);
	}
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
