/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor2.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/16 12:53:44 by seyildir      #+#    #+#                 */
/*   Updated: 2024/04/16 12:53:44 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	child(t_data *data, t_command *cmd, int old_in, pid_t *pip)
{
	int	b;

	do_sig(CHILD);
	close(pip[0]);
	if (old_in != -1)
	{
		dup2(old_in, STDIN_FILENO);
		close(old_in);
	}
	if (cmd->next)
		dup2(pip[1], STDOUT_FILENO);
	close(pip[1]);
	if (cmd->files)
		if (setio(cmd->files))
			exit(127);
	if (!data->cmd || (!data->cmd->args && !data->cmd->files))
		exit(SUCCESS);
	b = builtin_com(cmd);
	if (b != -1)
		exit(b);
	run_com(cmd, data->env);
}

int	get_exit_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (SUCCESS);
}

int	fork_run(t_command *cmd, t_env *env)
{
	pid_t	pid;

	do_sig(PARENT);
	pid = fork();
	if (pid == -1)
		return (FAIL);
	if (!pid)
		run_com(cmd, env);
	return (get_exit_status(pid));
}

int	single_exec(t_data *data)
{
	int	b;
	int	o_stdin;
	int	o_stdout;

	if (!data->cmd || (!data->cmd->args && !data->cmd->files))
		return (SUCCESS);
	if (data->cmd->files)
	{
		o_stdin = dup(STDIN_FILENO);
		o_stdout = dup(STDOUT_FILENO);
		if (setio(data->cmd->files))
			return (1);
	}
	if (data->cmd->args)
	{
		b = builtin_com(data->cmd);
		if (b == -1)
			b = fork_run(data->cmd, data->env);
	}
	if (data->cmd->files)
	{
		dup2(o_stdout, STDOUT_FILENO);
		dup2(o_stdin, STDIN_FILENO);
		close(o_stdin);
		close(o_stdout);
	}
	return (b);
}

int	executor(t_data *data)
{
	t_command	*tmp;
	pid_t		pid;
	int			pip[2];
	int			old_in;

	tmp = data->cmd;
	old_in = -1;
	if (!data->cmd->next)
		return (single_exec(data));
	do_sig(PARENT);
	while (tmp)
	{
		if (pipe(pip) == -1)
			return (FAIL);
		pid = fork();
		if (pid == -1)
			return (FAIL);
		if (!pid)
			child(data, tmp, old_in, pip);
		if (old_in != -1)
			close(old_in);
		old_in = pip[0];
		close(pip[1]);
		tmp = tmp->next;
	}
	close(old_in);
	return (get_exit_status(pid));
}
