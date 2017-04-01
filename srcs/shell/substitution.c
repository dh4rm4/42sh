#include "shell.h"

static void	manage_quote(char *quote, char current)
{
	if (!(*quote))
		*quote = current;
	else if (current == *quote)
		*quote = '\0';
}

static int	is_ret_last_cmd_ret(char *line, int i)
{
	if (!line[i + 1])
		return (0);
	if (line[i] == '$' && line[i + 1] == '?' &&
		(line[i + 2] == ' ' || !line[i + 2]))
		return (1);
	return (0);
}

void		do_substitution(t_env *e, int *curr_pos, char *substitute,
							int nb_char_to_jump)
{
	char	*new;
	char	*tmp;
	char 	*ret;

	new = ft_strnew((int)(ft_strlen(e->line) + (int)ft_strlen(substitute)));
	ret = new;
	tmp = e->line;
	while (*e->line)
	{
		if (e->line == &tmp[*curr_pos] && substitute)
			ft_replace_word(&new, substitute, &e->line, nb_char_to_jump + 1);
		else
		{
			*new = *e->line;
			new++;
		}
		e->line++;
	}
	strfree(&tmp);
	e->line = ret;
}

int			substitution(t_env *e)
{
	int		i;
	char	quote;
	int		ret;
	char	*user_dir;

	i = -1;
	ret = 0;
	quote = '\0';
	user_dir = ft_strdup("/Users/");
	if (e->line[0] && e->line[0] == '~')
	{
		ft_strdel(&user_dir);
		return (ft_error(SH_NAME, e->home, ": is a directory"));
	}
	while (e->line[++i])
	{
		if (ret == -1)
			return (-1);
		if ((e->line[i] == '"' || e->line[i] == '\'') && i - 1 >= 0 &&
			e->line[i - 1] != '\\')
			manage_quote(&quote, e->line[i]);
		else if (e->line[i] == '~' && i && e->line[i - 1] == ' '
				&& e->line[i + 1] && e->line[i + 1] != ' '
				&& e->line[i + 1] != '/')
			do_substitution(e, &i, user_dir, 0);
		//else if (e->line[i] == '~' && !quote && i && e->line[i - 1] == ' ')
		else if (e->line[i] == '~' && !quote && i && e->line[i - 1] == ' ')
			do_substitution(e, &i, e->home, 0);
		else if (e->line[i] == '!' && !quote)
			ret = manage_exclamation_mark(e, &i);
		else if (e->line[i] == '$' && e->line[i + 1])
			ret = do_env_subs(e, &i);
		else if (is_ret_last_cmd_ret(e->line, i))
			do_substitution(e, &i, e->last_ret, 2);
	}
	strfree(&user_dir);
	if (ret)
		ft_printf("%s\n", e->line);
	return (ret);
}
