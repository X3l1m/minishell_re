/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd_com.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/04 13:03:26 by seyildir      #+#    #+#                 */
/*   Updated: 2024/03/04 13:03:27 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	pwd_com(t_data *data)
{
	ft_putendl_fd(data->cwd, STDOUT_FILENO);
	return (SUCCESS);
}
