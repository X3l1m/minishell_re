/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/04 12:59:54 by seyildir      #+#    #+#                 */
/*   Updated: 2024/03/04 12:59:54 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*join_str(char *str, char *add)
{
	char	*tmp;

	if (!add)
		return (str);
	if (!str)
		return (ft_strdup(add));
	tmp = str;
	str = ft_strjoin(tmp, add);
	free(tmp);
	return (str);
}

int	error_mini(char *errmsg, int num)
{
	ft_putstr_fd("Error\n", STDERR_FILENO);
	if (errno == 0)
		ft_putendl_fd(errmsg, STDERR_FILENO);
	else
		perror(errmsg);
	if (num < 0)
		exit(EXIT_FAILURE);
	else
		return (num);
}

int	err_msg(char *com, char *msg, int err_nm)
{
	ft_putstr_fd(com, STDERR_FILENO);
	write(STDERR_FILENO, ": ", 2);
	ft_putendl_fd(msg, STDERR_FILENO);
	return (err_nm);
}

int	cmd_err_msg(char *command, char *info, char *msg, int err)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (info)
	{
			ft_putstr_fd("`", STDERR_FILENO);
		ft_putstr_fd(info, STDERR_FILENO);
		ft_putstr_fd("'", STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
	return (err);
}

bool	print_error_msg(char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '",
		STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return (false);
}
