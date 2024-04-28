/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/10 18:17:05 by seyildir      #+#    #+#                 */
/*   Updated: 2023/04/28 19:11:32 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

long	ft_atoi(const char *str)
{
	short int	sy;
	int			i;
	long		r;

	r = 0;
	i = 0;
	sy = 1;
	while ((str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
			|| str[i] == '\f' || str[i] == '\r' || str[i] == ' '))
		i++;
	if (str[i] == '-')
		sy = -sy;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
		r = (r * 10) + (str[i++] - '0');
	return (r * sy);
}

/* int main()
{
char *n = "2147483648";
int i1 = atoi(n);
long i2 = ft_atoi(n);

printf("%ld", i2);
if (i1 == i2)
write(1, "yes", 3);
else
write(1, "no", 2);
} */