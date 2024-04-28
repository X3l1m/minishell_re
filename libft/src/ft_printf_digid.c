/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf_digid.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/18 19:01:57 by seyildir      #+#    #+#                 */
/*   Updated: 2023/06/01 22:37:57 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	dec_convert(unsigned long int dec)
{
	unsigned long int	i;
	int					len;

	len = 0;
	i = dec % 10;
	if (dec >= 10)
		len += dec_convert(dec / 10);
	i += 48;
	len += write(1, &i, 1);
	return (len);
}

int	hex_convert(unsigned long int arg, char typef)
{
	int	i;
	int	len;

	len = 0;
	i = arg % 16;
	if (arg >= 16)
		len += hex_convert(arg / 16, typef);
	if (i < 10)
		i += 48;
	else
	{
		if (typef == 'X')
			i += 55;
		else
			i += 87;
	}
	len += write(1, &i, 1);
	return (len);
}

int	flg_decimal(va_list arg)
{
	int	arg_dec;
	int	len;

	len = 0;
	arg_dec = va_arg(arg, int);
	if (arg_dec == -2147483648)
		return (write(1, "-2147483648", 11));
	if (arg_dec < 0)
	{
		write(1, "-", 1);
		arg_dec = -arg_dec;
		len++;
	}
	len += dec_convert(arg_dec);
	return (len);
}

int	flg_hexa(va_list arg, char typef)
{
	unsigned int	arg_hex;

	arg_hex = va_arg(arg, unsigned int);
	return (hex_convert(arg_hex, typef));
}

int	flg_unsigned(va_list arg)
{
	unsigned int	arg_uns;

	arg_uns = va_arg(arg, unsigned int);
	return (dec_convert(arg_uns));
}
