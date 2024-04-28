/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/18 10:37:31 by seyildir      #+#    #+#                 */
/*   Updated: 2023/05/30 22:06:18 by ubuntu        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	flg_char(va_list arg)
{
	char	arg_c;

	arg_c = va_arg(arg, int);
	write(1, &arg_c, 1);
	return (1);
}

int	flg_string(va_list arg)
{
	int		len;
	char	*arg_s;

	len = 0;
	arg_s = va_arg(arg, char *);
	if (!arg_s)
		return (write(1, "(null)", 6));
	while (arg_s[len])
		len++;
	write(1, arg_s, len);
	return (len);
}

int	flg_pointer(va_list arg)
{
	unsigned long int	arg_point;

	arg_point = va_arg(arg, unsigned long int);
	if (!arg_point)
		return (write(1, "0x0", 3));
	return (write(1, "0x", 2) + hex_convert(arg_point, 'p'));
}

int	ptf_percent(char typef, va_list arg)
{
	int	len;

	len = 0;
	if (typef == 'c')
		len += flg_char(arg);
	else if (typef == 's')
		len += flg_string(arg);
	else if (typef == 'p')
		len += flg_pointer(arg);
	else if (typef == 'd' || typef == 'i')
		len += flg_decimal(arg);
	else if (typef == 'u')
		len += flg_unsigned(arg);
	else if (typef == 'X' || typef == 'x')
		len += flg_hexa(arg, typef);
	else if (typef == '%')
		len += write(1, "%", 1);
	else
	{
		len += write(1, "%", 1);
		len += write(1, &typef, 1);
	}
	return (len);
}

int	ft_printf(const char *typef, ...)
{
	va_list	arg;
	int		len;

	len = 0;
	va_start(arg, typef);
	while (*typef)
	{
		while (*typef == '%')
		{
			if (!*++typef)
				break ;
			len += ptf_percent(*typef, arg);
			if (!*++typef)
				break ;
		}
		if (!*typef)
			break ;
		write(1, typef++, 1);
		len++;
	}
	va_end(arg);
	return (len);
}
