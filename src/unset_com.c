/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset_com.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/04 13:03:25 by seyildir      #+#    #+#                 */
/*   Updated: 2024/03/04 13:03:25 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	env_remove(t_data *data, char *s)
{
	t_env	*tmp;
	t_env	*cur;

	cur = data->env;
	if (!cur)
		return ;
	while (cur)
	{
		if (cur->next && ft_strcmp(cur->next->variable, s))
		{
			tmp = cur->next;
			if (tmp)
			{
				cur->next = tmp->next;
				free(tmp->value);
				free(tmp->variable);
				free(tmp);
			}
		}
		cur = cur->next;
	}
}

int	unset_com(t_command *cmd)
{

	int	i;

	i = 0;
	while (cmd->args[++i])
		env_remove(cmd->_data, cmd->args[i]);
	return (env_to_envp(cmd->_data));
}