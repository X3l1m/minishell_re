/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_com.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/04 13:03:40 by seyildir      #+#    #+#                 */
/*   Updated: 2024/03/04 13:03:41 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	cd_com(t_command *cmd)
{
	if (!cmd->args[1])
	{
		cmd_err_msg("cd", NULL, "no given arguments", 1);
		return (FAIL);
	}
	if (cmd->args[2])
	{
		cmd_err_msg("cd", NULL, "too many arguments", 1);
		return (FAIL);
	}
	if (chdir(cmd->args[1]) == -1)
	{
		ft_putstr_fd("minishel: ", STDERR_FILENO);
		perror("cd");
		return (FAIL);
	}
	free(cmd->_data->cwd);
	cmd->_data->cwd = getcwd(NULL, 0);
	if (!cmd->_data->cwd)
		return (FAIL);
	return (SUCCESS);
}
