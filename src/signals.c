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

static void	do_sig_c(int mode)
{
	if (mode == SIGINT)
	{
		g_exit = 130;
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void do_sig(int mode)
{
	if (mode == MAIN)
	{
		signal(SIGINT, do_sig_c);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == PARENT || mode == IGNORE)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
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