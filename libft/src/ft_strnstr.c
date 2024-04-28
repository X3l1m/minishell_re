/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strnstr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/10 17:15:54 by seyildir      #+#    #+#                 */
/*   Updated: 2023/04/06 10:03:51 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	a;

	if (!*needle)
		return ((char *)haystack);
	i = 0;
	while (haystack[i] && len > i)
	{
		a = 0;
		while (haystack[i + a] == needle[a] && len > i + a && haystack[i + a])
			a++;
		if (!needle[a])
			return ((char *)haystack + i);
		i++;
	}
	return (0);
}

/* int main()
{

	char *s = (ft_strnstr(haystack, needle, -1) == haystack + 1)
	printf("%s", s);
} */