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
#include <termios.h>
#include <unistd.h>

extern int g_exit;

// static void	init_shell(void)
// {
// 	struct termios	attributes;

// 	tcgetattr(STDIN_FILENO, &attributes);
// 	attributes.c_lflag &= ~(ECHOCTL);
// 	tcsetattr(STDIN_FILENO, TCSAFLUSH, &attributes);
// }


static void	do_sig_c(int mode)
{
	if (mode == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit = 130;
	}
}

static void	do_sig_heredoc(int mode)
{
	if (mode == SIGINT)
	{
		g_exit = 1;
		printf("> %s\n", rl_line_buffer);
		rl_replace_line("", 0);
		rl_on_new_line();
		exit(g_exit);
	}
}

void do_sig(int mode)
{
	signal(SIGQUIT, SIG_IGN);
	if (mode == MAIN)
		signal(SIGINT, do_sig_c);
	else if (mode == PARENT || mode == IGNORE)
		signal(SIGINT, SIG_IGN);
	else if (mode == CHILD)
		signal(SIGINT, do_sig_heredoc);
	else if (mode == HEREDOC)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}