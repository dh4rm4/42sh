#include "shell.h"

int			ft_matchquotes(char *s)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
	while (s[i])
	{
		if (quote != '\0' && s[i] == quote)
			quote = '\0';
		else if (quote == '\0' && (s[i] == '\'' || s[i] == '\"'))
			quote = s[i];
		++i;
	}
	if (quote != '\0')
		return (quote);
	return (0);
}

void		ft_env_free(t_env *e)
{
	if (e->line)
		strfree(&e->line);
	if (e->home)
		strfree(&e->home);
	if (TCAPS.term_name)
		strfree(&TCAPS.term_name);
	if (e->history)
		ft_free_tab(e->history);
	if (e->prompt)
		strfree(&e->prompt);
	if (e->env)
		ft_free_tab(e->env);
	if (e->magic)
		magic_free(e);
}

char		*ft_issetenv(char **env, char *name)
{
	char	*eval;
	int		i;

	i = -1;
	eval = ft_strjoin(name, "=");
	if (env)
	{
		while (env[++i])
		{
			if (ft_strnequ(env[i], eval, ft_strlen(eval)))
			{
				free(eval);
				return (env[i]);
			}
		}
	}
	free(eval);
	return (NULL);
}

char		*ft_getenv(char **env, char *name)
{
	char	*value;
	char	*tmp;

	value = NULL;
	tmp = NULL;
	if ((tmp = ft_issetenv(env, name)) != NULL)
	{
		value = ft_strdup(ft_strchr(tmp, '=') + 1);
	//	free(tmp);
	}
	return (value);
}

void	xputs(char *tag)
{
	char	*res;

	res = tgetstr(tag, NULL);
	tputs(res, 1, dsh_putchar);
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

void	strfree(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
}