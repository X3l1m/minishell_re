/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/26 22:46:04 by seyildir      #+#    #+#                 */
/*   Updated: 2023/06/01 22:38:08 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	size_t	p;
	size_t	p1;

	p = ft_strlen(s1);
	p1 = ft_strlen(s2) + 1;
	join = malloc(p + p1);
	if (!join)
		return (NULL);
	ft_memcpy(join, s1, p);
	ft_memcpy(join + p, s2, p1);
	return (join);
}

char	*ft_strjoin_free(char *s1, char *s2, int free_val)
{
	char	*new;

	new = ft_strjoin(s1, s2);
	if (free_val == 1 || free_val == 3)
		free(s1);
	if (free_val == 2 || free_val == 3)
		free(s2);
	return (new);
}

/* char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*join;
	size_t	p;
	size_t	p1;

	p = ft_strlen(s1);
	p1 = ft_strlen(s2) + 1;
	join = malloc(p + p1);
	if (!join)
		return (free(s1), NULL);
	ft_memcpy(join, s1, p);
	ft_memcpy(join + p, s2, p1);
	free(s1);
	return (join);
} */

/* int main()
{
	char s1[] = "ilk";
	char s2[] = "son";
	char *i = ft_strjoin(s1, s2);
	printf("%s\n", i);
} */