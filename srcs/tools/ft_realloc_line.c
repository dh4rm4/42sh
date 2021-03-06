/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/28 12:09:44 by lfabbro           #+#    #+#             */
/*   Updated: 2017/04/28 12:09:44 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*ft_realloc_delete_char(t_env *e, int pos)
{
	char	*new;
	int		len;
	int		i;
	int		j;

	new = NULL;
	if (!e->line)
		return (NULL);
	len = (int)ft_strlen(e->line);
	if ((!pos && len == 1) || !(new = ft_strnew(len)))
	{
		strfree(&e->line);
		return (NULL);
	}
	i = -1;
	j = -1;
	if (len >= 1)
		while (e->line[++i])
			if (i != pos)
				new[++j] = e->line[i];
	strfree(&e->line);
	return (new);
}

char	*ft_realloc_line(t_env *e, char c)
{
	char	*new;
	int		len;
	int		i;

	if (e->line)
		len = ft_strlen(e->line) + 1;
	else
		len = 1;
	if (!(new = ft_strnew(len)))
		return (NULL);
	i = 0;
	if (len > 1)
		while (e->line[i])
		{
			new[i] = e->line[i];
			++i;
		}
	new[i] = c;
	strfree(&e->line);
	return (new);
}

char	*ft_delete_char(char *str, int pos)
{
	char	*new;
	int		len;
	int		i;
	int		j;

	if (str)
		len = (int)ft_strlen(str) - 1;
	else
		return (NULL);
	if (!(new = ft_strnew(len)))
	{
		strfree(&str);
		return (NULL);
	}
	i = -1;
	j = -1;
	if (len >= 1)
		while (str[++i])
			if (i != pos)
				new[++j] = str[i];
	strfree(&str);
	return (new);
}

int		detect_tabs(char *str, int *i, int *j)
{
	int		k;
	int		count;

	k = -1;
	count = 0;
	*i = -1;
	*j = 0;
	while (str && str[++k])
		if (str[k] == '\t')
			++count;
	return (count);
}

char	*convert_tabs(char *str)
{
	int		i;
	int		j;
	int		count;
	char	*ret;

	if (!(count = detect_tabs(str, &i, &j)))
		return (str);
	if (!(ret = ft_strnew((int)ft_strlen(str) + (4 * count) - count)))
		return (NULL);
	while (str[++i])
	{
		if (str[i] != '\t')
			ret[j] = str[i];
		else
		{
			ret[j] = ' ';
			ret[++j] = ' ';
			ret[++j] = ' ';
			ret[++j] = ' ';
		}
		++j;
	}
	strfree(&str);
	return (ret);
}
