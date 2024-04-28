/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/04 13:03:38 by seyildir      #+#    #+#                 */
/*   Updated: 2024/03/04 13:03:39 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	do_dup2(t_file *tmp)
{
	if (tmp->type == Read || tmp->type == Here_doc)
	{
		tmp->fd = open(tmp->name, O_RDONLY);
		if (tmp->fd == -1)
			return(cmd_err_msg(tmp->name, NULL, "No such file or directory", 127));
		if (dup2(tmp->fd, STDIN_FILENO) == -1)
			perror("Error: dup2");
	}
	else if (tmp->type == Write)
	{
		tmp->fd = open(tmp->name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (tmp->fd == -1)
			return(cmd_err_msg(tmp->name, NULL, "No such file or directory", 127));
		if (dup2(tmp->fd, STDOUT_FILENO) == -1)
			perror("Error: dup2");
	}
	else if (tmp->type == Append)
	{
		tmp->fd = open(tmp->name, O_CREAT | O_APPEND | O_WRONLY, 0777);
		if (tmp->fd == -1)
			return(cmd_err_msg(tmp->name, NULL, "No such file or directory", 127));
		if (dup2(tmp->fd, STDOUT_FILENO) == -1)
			perror("Error: dup2");
	}
	return(SUCCESS);
}

int	setio(t_file *files)
{
	t_file	*tmp;

	tmp = files;
	while (tmp)
	{
		if (do_dup2(tmp))
			return (FAIL);
		if (tmp->fd != -1)
			close(tmp->fd);
		tmp = tmp->next;
	}
	return(SUCCESS);
}

void	exec_check(char *com, char *path)
{
	struct stat	info;

	if (stat(path, &info) == -1)
		exit(cmd_err_msg(com, NULL, "No such file or directory", 127));
	else if (S_ISDIR(info.st_mode))
		exit(cmd_err_msg(com, NULL, "is a directory", 126));
	else if ((info.st_mode & S_IXUSR) == 0)
		exit(cmd_err_msg(com, NULL, "Permission denied", 126));
}

int	run_com(t_command *cmd, t_env *env)
{
	char	*path;

	do_sig(CHILD);
	if (!cmd->args || !cmd->args[0])
		exit (0);
	if (!ft_strchr(cmd->args[0], '/'))
		path = pathf(cmd->args[0], env);
	else
		path = cmd->args[0];
	if (!path)
		exit (err_msg(cmd->args[0], "command not found", 127));
	exec_check(cmd->args[0], path);
	execve(path, cmd->args, cmd->_data->envp);
	perror(cmd->args[0]);
	exit(errno);
}

int	env_com(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		ft_putendl_fd(env[i++], STDOUT_FILENO);
	return (SUCCESS);
}

int	builtin_com(t_command *cmd)
{
	if (!cmd->args)
		return (-1);
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (echo_com(cmd));
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (cd_com(cmd));
	if (!ft_strcmp(cmd->args[0], "pwd"))
		return (pwd_com(cmd->_data));
	if (!ft_strcmp(cmd->args[0], "export"))
		return (export_com(cmd));
	if (!ft_strcmp(cmd->args[0], "unset"))
		return (unset_com(cmd));
	if (!ft_strcmp(cmd->args[0], "env"))
		return (env_com(cmd->_data->envp));
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (exit_com(cmd->_data));
	return (-1);
}
