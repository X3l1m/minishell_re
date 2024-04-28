/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_com.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/04 13:03:32 by seyildir      #+#    #+#                 */
/*   Updated: 2024/03/04 13:03:32 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_valid(char *str)
{
	int	v;

	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (-1);
	v = 0;
	while (str[++v])
	{
		if (str[v] == '=')
			return (v);
		if (!ft_isalnum(str[v]) && str[v] != '_')
			return (-1);
	}
	return (0);
}

void	export_print(t_env *env)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(tmp->variable, STDOUT_FILENO);
		if (tmp->equal)
		{
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putchar_fd('\"', STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putchar_fd('\"', STDOUT_FILENO);
		}
		write(1, "\n", 1);
		tmp = tmp->next;
	}
}

int	export_com(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[++i])
	{
		
		if (check_valid(cmd->args[i]) != -1)
		{
			if (env_lst_add(cmd->args[i], cmd->_data))
				return (FAIL);
		}
		else
			g_exit = cmd_err_msg("export",
					cmd->args[i], "not a valid identifier", 1);
	}
	if (!cmd->args[1])
		export_print(cmd->_data->env);
	return (g_exit);
}
