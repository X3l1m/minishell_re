/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo_com.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/04 13:03:39 by seyildir      #+#    #+#                 */
/*   Updated: 2024/03/04 13:03:40 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_n(char **str)
{
	int	i;
	int	nl;

	nl = 1;
	while (str[nl] && str[nl][0] == '-')
	{
		i = 1;
		if (!str[nl][i])
			return (nl - 1);
		while (str[nl][i])
			if (str[nl][i++] != 'n')
				return (nl - 1);
		nl++;
	}
	return (nl - 1);
}

int	echo_com(t_command *cmd)
{
	int		i;
	int		nl;

	if (!cmd->args[1])
	{
		write(1, "\n", 1);
		return (SUCCESS);
	}
	nl = check_n(cmd->args);
	i = nl;
	while (cmd->args[++i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			write(1, " ", 1);
	}
	if (!nl)
		write(1, "\n", 1);
	return (SUCCESS);
}
