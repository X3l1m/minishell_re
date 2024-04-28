/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/04 12:59:57 by seyildir      #+#    #+#                 */
/*   Updated: 2024/03/04 12:59:57 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

extern int g_exit;

// void	init_shell_envioment(void)
// {
// 	struct termios	attributes;

// 	tcgetattr(STDIN_FILENO, &attributes);
// 	attributes.c_lflag &= ~(ECHOCTL);
// 	tcsetattr(STDIN_FILENO, TCSAFLUSH, &attributes);
// }

static void	do_sig_c(int mode)
{
	(void)mode;
	write(1, "\n", 1);
	g_exit = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// static void	do_sig_heredoc(int mode)
// {
// 	(void)mode;
// 	printf("> %s\n", rl_line_buffer);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	exit(130);
// }

void do_sig(int mode)
{
	if (mode == MAIN)
	{
		// init_shell_envioment();
		signal(SIGINT, do_sig_c);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == PARENT || mode == IGNORE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == CHILD)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (mode == HEREDOC)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
	}
}