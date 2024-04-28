/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isalpha.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 20:14:11 by seyildir      #+#    #+#                 */
/*   Updated: 2023/04/06 10:03:51 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_isalpha(int c)
{
	return (('a' <= c && c <= 'z') | (c >= 'A' && c <= 'Z'));
}

/* int main()
{
	char c = '1';
 	printf("%d\n", isalpha(c));

	c = 'a';
	printf("%d\n", isalpha(c));

	c = '+';
	printf("%d\n", isalpha(c));

	printf("%d", ft_isalpha(6));
} */
