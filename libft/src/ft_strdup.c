/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strdup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 17:43:58 by seyildir      #+#    #+#                 */
/*   Updated: 2023/04/06 10:03:51 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strdup(char const *s1)
{
	char	*rtn;

	rtn = malloc(ft_strlen(s1) + 1);
	if (!rtn)
		return (NULL);
	rtn = ft_memcpy(rtn, s1, ft_strlen(s1) + 1);
	return (rtn);
}

/* int main()
{
	char *source;
	source = malloc(sizeof(char *) * 4);
	source[0] = '1';
	source[1] = '3';
	source[2] = '2';
	source[3] = '\0';
	char *target = ft_strdup(source);
	// printf("%s\n", source);
	// printf("%s\n", target);
	// printf("%p\n%p\n", source, target);
	int i =0;
	while(target[i])
	{
		printf("%c",target[i]);
		i++;
	}
} */