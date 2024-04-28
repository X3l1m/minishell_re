/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit_com.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/04 13:03:37 by seyildir      #+#    #+#                 */
/*   Updated: 2024/03/04 13:03:37 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	check_max(int i, long num, short sg)
{
	if (num > LONG_MAX / 10)
		return (false);
	else if (num == LONG_MAX / 10)
		if ((i * sg) > LONG_MAX % 10
			|| (i * sg) < LONG_MIN % 10)
			return (false);
	return (true);
}

bool	num_conv(char *str, int *exit_cod)
{
	int		i;
	long	num;
	short	sg;

	i = 0;
	sg = 1;
	num = 0;
	while ((str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
			|| str[i] == '\f' || str[i] == '\r' || str[i] == ' '))
		i++;
	if (str[i] == '-')
		sg = -sg;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!check_max(str[i] - 48, num, sg))
			return (false);
		num = (num * 10) + (str[i++] - 48);
	}
	num *= sg;
	num %= 256;
	*exit_cod = (int)num;
	return (true);
}

int	exit_com(t_data *data)
{

	ft_putendl_fd("exit", 1);
	if (data->cmd->args[1])
	{
		if (!ft_istrdigit(data->cmd->args[1])
			|| !num_conv(data->cmd->args[1], &data->status))
		{
			data->status = cmd_err_msg("exit", data->cmd->args[1], \
			"numeric argument required", 2);
		}
		else if (data->cmd->args[2])
			return (cmd_err_msg("exit", NULL, "too many arguments", 1));
	}
	data->end = 1;
	return (data->status);
}
