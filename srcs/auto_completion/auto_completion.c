#include "shell.h"

static char	*get_path_from_arg(char *arg)
{
	char	*path;
	int		i;
	int		x;

	i = ft_strlen(arg);
	while (i > 0 && arg[i - 1] != '/')
		--i;
	if (!arg || !i || (i > 0 && arg[i - 1] == ' '))
		return ((path = getcwd(NULL, 0)));
	path = ft_strnew(i);
	ft_strncpy(path, arg, i);
	x = 0;
	while (path[x])
	{
		if (path[x] == '\\')
			ft_strncpy(&path[x], &path[x + 1], i - x);
		x++;
	}
	return (path);
}

static char	*isolate_arg_to_complete(char *arg)
{
	int		i;
	int		j;
	char	*ret;

	i = ft_strlen(arg);
	if (!arg || !i)
		return (arg);
	j = 0;
	while (i > 0 && arg[i - 1] != '/')
		--i;
	if (!(ret = ft_strnew(ft_strlen(arg) - i)))
		return (NULL);
	while (arg[i])
		ret[j++] = arg[i++];
	return (ret);
}

void		*ft_free_double_array(char **array)
{
	int i;

	i = 0;
	if (array)
	{
		while (array[i])
		{
			ft_strdel(&array[i]);
			i++;
		}
		free(array);
	}
	return (NULL);
}

char		*add_backquote(t_env *e, char *str, int i)
{
	int x;

	x = i;
	if (cur_inquote(e))
	{
		while (x > 0 && str[x] != ' ' && str[x] != '\'' && str[x] != '\"')
			x--;
		if (str[x] != '\'' || str[x] != '\"')
			x++;
	}
	else
	{
		while (x > 0 && (str[x] != ' ' || str[x - 1] == '\\'))
			x--;
	}
	if (str[x] == ' ')
		x++;
	return (ft_strsub(str, x, i - x + 1));
}

int			auto_completion(t_env *e)
{
	char	*arg_comp;

	if (!e->line)
		return (0);
	if (e->selected >= -1 && e->files)
	{
		e->selected = e->files[e->selected + 1] ? e->selected + 1 : 0;
		print_auto_completion(e, NULL, NULL, NULL);
	}
	else
	{
		arg_comp = add_backquote(e, e->line, TCAPS.nb_move - 1);
		complete_arg(e, arg_comp);
		ft_strdel(&arg_comp);
	}
	return (1);
}
