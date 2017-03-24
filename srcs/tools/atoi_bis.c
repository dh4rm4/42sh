#include "shell.h"
#include <limits.h>

int		atoi_bis(const char *str)
{
	int		n;
	int		sign;

	n = 0;
	while (*str == ' ' || *str == '\n' || *str == '\t' || *str == '\r' ||
			*str == '\f' || *str == '\v')
		++str;
	sign = (*str == '-') ? -1 : 1;
	if (*str == '+' || *str == '-')
		++str;
	while (*str >= '0' && *str <= '9')
	{
		n = n * 10 + (*str - 48) % 10;
		if (n < 0 || n > INT_MAX)
			return (-1);
		++str;
	}
	return (n * sign);
}
