/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strcmp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/10 21:46:54 by seyildir      #+#    #+#                 */
/*   Updated: 2023/06/03 20:03:12 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2 && *s1 && *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n <= 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] && s2[i] && n > i + 1)
		i++;
	return (s1[i] - s2[i]);
}

/* #include <stdio.h>
#include <string.h>

int main()
{
	char a[] = "";
	char b[] = "";
	printf("%d\n", ft_strcmp(a, b));
	printf("%d\n", strcmp(a, b));
} */