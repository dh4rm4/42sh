#include "shell.h"

int	redir_from_hdoc(t_env *e)
{
	int		i;
	int		len;
	char	**tmp;
	int		newfd[2];
	int		ret;
	char	buf[4096];

	i = -1;
	tmp = NULL;
	if (e->hdoc && e->hdoc->content)
		tmp = (char **)e->hdoc->content;
	if (!tmp || !tmp[i + 1])
		return (0);
	pipe(newfd);
	if (e->check_input)
	{
		while ((ret = read(FD.in, &buf, 4095)) > 0)
		{
			buf[ret] = '\0';
			ft_putstr_fd(buf, newfd[1]);
		}
		ft_close(FD.in);
	}
	while (tmp[++i])
	{
		len = (int)ft_strlen(tmp[i]);
		if (tmp[i][len] != '\\')
			ft_putendl_fd(tmp[i], newfd[1]);
		else
			ft_putstr_fd(tmp[i], newfd[1]);
	}
	FD.in = newfd[0];
	ft_close(newfd[1]);
	e->hdoc = e->hdoc->next;
	e->check_input = 1;
	return (1);
}
