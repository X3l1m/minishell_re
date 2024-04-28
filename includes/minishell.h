#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

#define HERE_DOC_NAME "/tmp/minishell_heredoc_"
#define FORK_FAIL 127
#define PARS_FAIL 2
#define CWD_FAIL 1
#define MAL_FAIL 2
#define SUCCESS 0
#define FAIL 1
#define PARS_DELIMS "<>| "
#define TRUE 1
#define FALSE 0
#define LOGIC_ERROR 245
# define IGNORE 5
# define HEREDOC 4
# define PARENT 3
# define CHILD 2
# define MAIN 1

typedef struct s_env
{
	char			*variable;
	int				equal;
	char			*value;
	struct s_env	*next;
	
}	t_env;

enum FileType {
	Read,
	Write,
	Here_doc,
	Append
};

typedef struct s_file
{
	enum FileType	type;
	char			*delim;
	int				fd;
	int				index;
	char			*name;
	struct s_file	*next;
}	t_file;


typedef struct s_command
{
	char				**args;
	t_file				*files;
	struct s_data		*_data;
	struct s_command	*next;
}	t_command;


typedef struct s_data
{
	int			status;
	int			end;
	char		*line;
	char		*cwd;
	char		**envp;
	t_env		*env;
	t_command	*cmd;
} t_data;

extern int	g_exit;

// execute
int	executor(t_data *data);
int	cd_com(t_command *cmd);
int	echo_com(t_command *cmd);
int	get_exit_status(pid_t pid);
int	exit_com(t_data *data);
int	builtin_com(t_command *cmd);
int	run_com(t_command *cmd, t_env *env);
int	pwd_com(t_data *data);
int	unset_com(t_command *cmd);
int	export_com(t_command *cmd);
char	*pathf(char *com, t_env *env);
int		setio(t_file *files);

void	do_clean_args(char **args);

void	do_sig(int mode);

int	err_msg(char *com, char *msg, int err_nm);
t_env	*env_find(char *name, t_env *head);
int		env_lst_add(char *s, t_data *data);
int	error_mini(char *errmsg, int num);
int	cmd_err_msg(char *command, char *info, char *msg, int err);
bool	print_error_msg(char *str);

int	env_to_envp(t_data *data);


// int	export_com(t_command *cmd, t_dllist *env);
// int	update_evn(t_dllist *env);
// int	unset_com(t_command *cmd, t_dllist *env);
// t_dlnode	*find_env(t_dllist *env, char *name);
// char	*pathf(char *com, t_dllist *env);




#endif