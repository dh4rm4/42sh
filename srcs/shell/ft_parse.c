#include "shell.h"

static int		exec_by_type(t_env *e, int i, int ret)
{
//	while (is_aggregator(e, RED_INDEX) || is_output_redir(e, RED_INDEX)
//			|| is_heredoc(e, RED_INDEX))
//		struct_find_red(e);
	if (!e->cat[i + 1] && redir_check_red(e, "|")
		&& is_next_redir(e, RED_INDEX) == OUTPUT)
		ret = redir_exec_open(i, e);
	else if (!e->cat[i + 1] && redir_check_red(e, "|"))
	{
		FD.fd[1] = STDOUT_FILENO;
		if (is_next_redir(e, RED_INDEX) == AGGREGATOR)
			struct_find_red(e);
		ret = ft_exec_cmd(e, e->cat[i]);
	}
	else
		ret = redir_exec_open(i, e);
	if (is_output_redir(e, RED_INDEX))
		redir_fill_output(e);
	dup2(FD.stdin, STDIN_FILENO);
	dup2(FD.stdout, STDOUT_FILENO);
	dup2(FD.stderr, STDERR_FILENO);
	return (ret);
}

static void		exec_end(t_env *e)
{
	ft_waitsons(e);
	ft_triple_free(e);
	ft_free_tab(e->hdoc_words);
	e->hdoc_words = NULL;
	magic_free(e);
	RED_INDEX = 0;
	ft_tabfree(e->cmd);
	e->cmd = NULL;
}

int				ft_iter_cmds(t_env *e, char *cmds_i)
{
	int		i;
	int		ret;

	i = -1;
	ret = 0;
	FD.in = STDIN_FILENO;
	if (!(e->cmd = ft_strsplit_quote_bs(cmds_i, ' ')) ||
		!(e->magic = struct_strsplit_wo_quote_bs(cmds_i, ' ')))
		return (ft_error(SH_NAME, "parsing error.", NULL));
	if (magic_type(e) == -1)
		return (-42);
	if ((e->cat = ft_cmds_split(e)) == NULL)
		return (-1);
	ft_create_file(e);
/*	ft_printf("====  MAGIC  ====\n");
	for (int j = 0 ; e->magic[j].cmd ; j++)
		ft_printfd(2, "cmd[%d]: %s | type: %s\n", j, e->magic[j].cmd, e->magic[j].type);
	ft_printf("====   CAT       ====\n");
	for (int k = 0 ; e->cat[k] ; ++k)
		for (int l = 0 ; e->cat[k][l] ; ++l)
			ft_printf("cat[%d][%d]: %s\n", k, l, e->cat[k][l]);
	ft_printf("====  END CAT    ====\n");
*/	while (++i < ft_catlen(e->cat) && e->cat[i])
	{
		ret = exec_by_type(e, i, ret);
//		reset_last_ret(e, ret);
		i += manage_operators(e, RED_INDEX, ret);
	}
	exec_end(e);
	return (ret);
}

int				ft_parse_line(t_env *e)
{
	int		i;
	int		ret;
	char	**cmds;

	i = -1;
	ret = 0;
	ft_store_history(e);
	if (substitution(e) == -1)
		return (-1);
	if ((cmds = ft_trim_split_cmd(e)) != NULL)
	{
		if (!cmds[0])
			ft_printfd(2, "%s: syntax error near unexpected token \";\"\n",
					   SH_NAME);
		while (cmds[++i])
		{
			ret = ft_iter_cmds(e, cmds[i]);
			if (ret == -42)
				return (ret);
			tcaps_set(e);
		}
	}
	ft_free_tab(cmds);
	return (ret);
}
