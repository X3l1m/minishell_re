#include <minishell.h>

int	do_expand_arg(t_data *data, char **new, char *arg);

int	g_exit = 0;

int	set_exit(int val)
{
	g_exit = val;
	return (val);
}

void	*set_exit_null(int val)
{
	g_exit = val;
	return (NULL);
}

int	get_array_size(char **arr)
{
	int i;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	token_add(char ***args, char *new)
{
	char	**local;
	int		i;
	int		size;

	if (!args)
		return (set_exit(LOGIC_ERROR));
	size = get_array_size(*args);
    local = ft_calloc(sizeof(char*), size + 2);
	if (!local)
		return (set_exit(MAL_FAIL));
	i = -1;
	while (++i < size)
		local[i] = (*args)[i];
	local[i] = new;
	local[i + 1] = NULL;
	if (*args)
		free(*args);
	*args = local;
	return(SUCCESS);
}

int	str_join_or_dup(char **s1, char *s2, int type)
{
	if (*s1)
		*s1 = ft_strjoin_free(*s1, s2, type);
	else
		*s1 = ft_strdup(s2);
	if (!(*s1))
		return (set_exit(MAL_FAIL));
	return (SUCCESS);
}

void    print_args(char **s)
{
	if (!s)
		return ;
	int i = -1;
	while (s[++i])
		printf("arg: \"%s\"\n", s[i]);
}

void    print_files(t_file *file)
{
	t_file    *tmp;

	tmp = file;
	while (tmp)
	{
		printf("file: fd=%d, name=%s, index=%d, type=%d, delim=%s\n", tmp->fd, tmp->name, tmp->index, tmp->type, tmp->delim);
		tmp = tmp->next;
	}
}

void    print_env(t_env    *env)
{
	t_env    *tmp;

	tmp = env;
	while (tmp)
	{
		printf("var: name=%s, sign=%d, value=%s\n", tmp->variable, tmp->equal, tmp->value);
		tmp = tmp->next;
	}
}


char	*ft_strndup(char const *s1, int n)
{
	char	*rtn;

	if (!s1)
		return (NULL);
	rtn = ft_calloc(1, n + 1);
	if (!rtn)
		return (NULL);
	rtn = ft_memcpy(rtn, s1, n);
	if (rtn[n])
		rtn[n] = 0;
	return (rtn);
}

void	give_err(char *text, char *point)
{
	write(2, "-minishell: ", 12);
	write(2, text, ft_strlen(text));
	write(2, "`", 1);
	if (!point || !*point || !ft_strcmp(point, "\n"))
		write(2, "newline", 7);
	else
		write(2, point, ft_strlen(point));
	write(2, "'\n", 2);
}

void	init_data(t_data *data)
{
	data->end = FALSE;
	data->cmd = NULL;
	data->cwd = NULL;
	data->line = NULL;
	data->envp = NULL;
	data->env = NULL;
	data->status = 0;
	g_exit = 0;
}

t_env	*env_lst_last(t_env *node)
{
	t_env	*tmp;

	if (!node)
		return (NULL);
	tmp = node;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);	
}

int	env_assign_var(char *s, t_env *new)
{
	int	i;

	if (!new)
		return (set_exit(MAL_FAIL));
	free(new->variable);
	free(new->value);
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	if (!s[i])
	{
		new->value = NULL;
		new->variable = ft_strdup(s);
		if (!new->variable)
			return (set_exit(MAL_FAIL));
		return (SUCCESS);
	}
	new->equal = TRUE;
	new->variable = ft_strndup(s, i);
	if (!new->variable)
		return (set_exit(MAL_FAIL));
	new->value = ft_strdup(&s[i + 1]);
	if (!new->value)
		return (set_exit(MAL_FAIL));
	return (SUCCESS);
}

int	env_lst_add(char *s, t_data *data)
{
	t_env	*tmp;

	if (env_find(s, data->env))
		return (env_assign_var(s, env_find(s, data->env)));
	tmp = ft_calloc(1, sizeof(t_env));
	if (!tmp)
		return (set_exit(MAL_FAIL));
	tmp->next = NULL;
	tmp->value = NULL;
	tmp->variable= NULL;
	if (data->env)
		env_lst_last(data->env)->next = tmp;
	else
		data->env = tmp;
	if (env_assign_var(s, tmp))
		return (MAL_FAIL);
	return (SUCCESS);
}

t_env	*env_find(char *name, t_env *head)
{
	t_env	*tmp;

	if (!head)
		return (NULL);
	tmp = head;
	while (tmp)
	{
		if (!ft_strcmp(tmp->variable, name))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	envp_to_env(char **envp, t_data *data)
{
	int		i;

	if (!envp)
		return (SUCCESS);
	i = 0;
	while (envp[i])
	{
		if (env_lst_add(envp[i], data))
			return (MAL_FAIL);
		i++;
	}
	return (SUCCESS);
}

int	env_shlvl_update(t_data *data)
{
	t_env	*tmp;
	int		val;

	tmp = env_find("SHLVL", data->env);
	if (!tmp)
	{
		if (env_lst_add("SHLVL=1", data))
			return (MAL_FAIL);
		return (SUCCESS);
	}
	val = ft_atoi(tmp->value);
	free(tmp->value);
	tmp->value = ft_itoa(val);
	if (!(tmp->value))
		return (set_exit(MAL_FAIL));
	return (SUCCESS);
}

int	env_pwd_update(t_data *data, char *cwd)
{
	t_env	*tmp;
	char	*holder;

	tmp = env_find("PWD", data->env);
	if (tmp)
	{
		free(tmp->value);
		tmp->value = ft_strdup(cwd);
		if (!tmp->value)
			return (set_exit(CWD_FAIL));
		return (SUCCESS);
	}
	holder = ft_strjoin("PWD=", cwd);
	if (!holder)
		return (set_exit(MAL_FAIL));
	if (env_lst_add(holder, data))
		return (free(holder), MAL_FAIL);
	free(holder);
	return (SUCCESS);
}

int	env_update(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (set_exit(CWD_FAIL));
	if (env_pwd_update(data, cwd))
		return (free(cwd), MAL_FAIL);
	free(data->cwd);
	data->cwd = cwd;
	if (env_shlvl_update(data))
		return (MAL_FAIL);
	return (SUCCESS);
}

int	env_size(t_env *env)
{
	t_env	*tmp;
	int		size;

	if (!env)
		return (0);
	size = 0;
	tmp = env;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return (size);
}

int	envp_assign(t_data *data, t_env *current)
{
	char	*tmp;

	if (!(current->equal))
		tmp = ft_strdup(current->variable);
	else
	{
		tmp = ft_strjoin(current->variable, "=");
		if (!tmp)
			return (set_exit(MAL_FAIL));
		if (current->value)
			tmp = ft_strjoin_free(tmp, current->value, 1);
	}
	if (!tmp)
		return (set_exit(MAL_FAIL));
	if (token_add(&(data->envp), tmp))
		return (set_exit(MAL_FAIL));
	return (SUCCESS);
}

void	do_clean_envp(t_data *data)
{
	int	i;

	if (!(data->envp))
		return ;
	i = 0;
	while (data->envp[i])
		free(data->envp[i++]);
	free(data->envp);
	data->envp = NULL;
}

void	do_clean_env(t_data *data)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp)
	{
		if (tmp->value)
			free(tmp->value);
		if (tmp->variable)
			free(tmp->variable);
		data->env = data->env->next;
		free(tmp);
		tmp = data->env;
	}
	data->env = NULL;
}

void	do_clean_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

void	do_clean_files(t_file *file)
{
	t_file	*tmp;
	t_file	*cur;

	if (!file)
		return ;
	tmp = file;
	while (tmp)
	{
		if (tmp->type == Here_doc)
			unlink(file->name);
		cur = tmp;
		tmp = tmp->next;
		free(cur->delim);
		free(cur->name);
		free(cur);
	}
}

void	do_clean_commands(t_data *data)
{
	t_command	*tmp;

	tmp = data->cmd;
	while (tmp)
	{
		data->cmd = tmp->next;
		do_clean_args(tmp->args);
		do_clean_files(tmp->files);
		free(tmp);
		tmp = data->cmd;
	}
	data->cmd = NULL;
}

void	do_clean_data(t_data *data)
{
	if (data->cwd)
		free(data->cwd);
	do_clean_env(data);
	do_clean_envp(data);
	if (data->line)
		free(data->line);
	do_clean_commands(data);
}

int	env_to_envp(t_data *data)
{
	int		i;
	t_env	*tmp;

	do_clean_envp(data);
	if (!data->env)
		return (SUCCESS);
	tmp = data->env;
	i = -1;
	while (tmp)
	{
		i++;
		if (envp_assign(data, tmp))
			return (set_exit(MAL_FAIL));
		tmp = tmp->next;
	}
	return (SUCCESS);
}

int	input_loop(t_data *data)
{
	int	i;

	while (1)
	{
		rl_on_new_line();
		data->line = readline("\033[0;35mminishell$\033[0m ");
		if (!data->line)
			break ;
		i = 0;
		while (data->line[i] && data->line[i] == ' ')
			i++;
		if (data->line[i])
			return (SUCCESS);
		free(data->line);
	}
	rl_clear_history();
	data->end = TRUE;
	write(1, "exit\n", 5);
	return (FAIL);
}

void	token_cut_helper(t_data *data, int *i, int *needle, char **token)
{
	*needle = *i + 2;
	if (data->line[*i] == '>' && data->line[*i + 1] == '>')
		*token = ft_strdup(">>");
	else if (data->line[*i] == '<' && data->line[*i + 1] == '<')
		*token = ft_strdup("<<");
	else
	{
		(*needle)--;
		if (&(data->line[*i]))
			*token = ft_strndup(&(data->line[*i]), 1);
		else
			*token = ft_strdup(&(data->line[*i]));
	}
	*i = *needle;
}

char	*token_cut(t_data *data, int *i, int *needle, int check)
{
	char	*token;

	if (check)
		return (give_err("MisMatching quote!", "Parsing Failure"), set_exit_null(2));
	if (ft_strchr(PARS_DELIMS, data->line[*i]))
		token_cut_helper(data, i, needle, &token);
	else
	{
		token = ft_strndup(&(data->line[*i]), *needle - *i);
		*i = *needle;
	}
	if (!token)
		return (set_exit_null(MAL_FAIL));
	return (token);
}

char	*tokenizer(t_data *data, int *i, int *needle)
{
	int		q_single;
	int		q_double;

	q_single = 0;
	q_double = 0;
	while(data->line[*needle])
	{
		if (!q_single && data->line[*needle] == '\"')
			q_double = !q_double;
		else if (!q_double && data->line[*needle] == '\'')
			q_single = !q_single;
		else if (!q_single && !q_double && ft_strchr(PARS_DELIMS, data->line[*needle]))
		{
			if (data->line[*i] == ' ' && *i == *needle)
				(*i)++;
			else
				break ;
		}
		(*needle)++;
	}
	return (token_cut(data, i, needle, q_single != 0 || q_double != 0));
}

t_file	*get_file_last(t_command *node)
{
	t_file	*tmp;

	if (!node)
		return (NULL);
	tmp = node->files;
	if (!tmp)
		return (tmp);
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

int do_heredoc_expand_arg(t_data *data, char **line)
{
	char	*new;

	new = NULL;
	if (do_expand_arg(data, &new, *line))
		return (free(new), g_exit);
	free(*line);
	*line = new;
	return (SUCCESS);
}

int	do_heredoc_child(t_data *data, t_file *file)
{
	char	*line;

	do_sig(HEREDOC);
	if (file->fd < 0)
		return (errno);
	while (1)
	{
		rl_on_new_line();
		line = readline("> ");
		if (!line)
		{
			give_err("here-document delimited by end-of-file ", file->delim);
			break ;
		}
		if (do_heredoc_expand_arg(data, &line))
			return (free(line), g_exit);
		if (!ft_strcmp(line, file->delim))
			break ;
		ft_putendl_fd(line, file->fd);
		free(line);
	}
	if (line)
		free(line);
	return (SUCCESS);
}

int	do_heredoc(t_data *data, t_file *file)
{
	pid_t	pid;

	do_sig(IGNORE);
	file->fd = open(file->name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	pid = fork();
	if (pid == -1)
		return (set_exit(FORK_FAIL));
	if (pid == 0)
		exit(do_heredoc_child(data, file));
	file->fd = close(file->fd);
	waitpid(pid, &g_exit, 0);
	if (WIFEXITED(g_exit))
		g_exit = WEXITSTATUS(g_exit);
	else if (WIFSIGNALED(g_exit))
		g_exit = WTERMSIG(g_exit);
	do_sig(MAIN);
	return (g_exit);
}

int	do_type_assign(t_file *file, char *type)
{
	if (!ft_strcmp("<", type))
		file->type = Read;
	else if (!ft_strcmp(">", type))
		file->type = Write;
	else if (!ft_strcmp(">>", type))
		file->type = Append;
	else if (!ft_strcmp("<<", type))
		file->type = Here_doc;
	return (SUCCESS);
}

t_file	*add_new_file(t_command *cur)
{
	t_file	*tmp;

	if (!cur)
		return (set_exit_null(LOGIC_ERROR));
	if (cur->files)
	{
		tmp = get_file_last(cur);
		tmp->next = ft_calloc(1, sizeof(t_file));
		tmp->next->index = tmp->index + 1;
		tmp = tmp->next;
	}
	else
	{
		cur->files = ft_calloc(1, sizeof(t_file));
		tmp = cur->files;
		tmp->index = 0;
	}
	if (!tmp)
		return (set_exit_null(MAL_FAIL));
	return (tmp);
}

int	do_add_file(t_command *cur, char **tokens, int i)
{
	t_file	*new;

	if (!(tokens[i + 1]) || !ft_strcmp(tokens[i + 1], "|")
		|| !ft_strcmp(tokens[i + 1], ">") || !ft_strcmp(tokens[i + 1], "<")
		|| !ft_strcmp(tokens[i + 1], ">>") || !ft_strcmp(tokens[i + 1], "<<"))
		return (give_err("syntax error near unexpected token", tokens[i]), set_exit(PARS_FAIL));
	new = add_new_file(cur);
	if (!new)
		return (g_exit);
	new->fd = -1;
	new->name = ft_strdup(tokens[i + 1]);
	if (!new->name)
		return (set_exit(MAL_FAIL));
	if (do_type_assign(new, tokens[i]))
		return (g_exit);
	return (SUCCESS);
}

int	do_command_jump(t_data *data, t_command **cur, char *next)
{
	if (!cur || !*cur)
		return (set_exit(MAL_FAIL));
	if (!next)
		return (give_err("syntax error near unexpected token", "|"), set_exit(PARS_FAIL));
	(*cur)->next = ft_calloc(1, sizeof(t_command));
	if (!((*cur)->next))
		return (set_exit(MAL_FAIL));
	(*cur) = (*cur)->next;
	(*cur)->_data = data;
	return (SUCCESS);
}

int	get_var_len(char *s)
{
	int	i;

	if (!s)
		return (0);
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (0);
	i = 0;
	while (s[++i] && (ft_isdigit(s[i]) || ft_isalpha(s[i]) || s[i] == '_'))
		;
	return (i);
}

int	do_quote_cut(char *s, char **old, int *quo, int *index)
{
	int		i;
	char	*tmp;

	*quo = 0;
	if (!s || !(s[1]))
		return (FAIL);
	i = 1;
	while (s[i] && s[i] != '\'')
		i++;
	if (!s[i])
		return (FAIL);
	*index += i + 1;
	if (i == 1)
		return (SUCCESS);
	tmp = ft_strndup(&(s[1]), i - 1);
	if (!tmp)
		return (set_exit(MAL_FAIL));
	if (str_join_or_dup(old, tmp, 3))
		return (g_exit);
	return (SUCCESS);
}

int	do_expand_sign(char **old, int *index)
{
	if (str_join_or_dup(old, "$", 1))
		return (g_exit);
	(*index)++;
	return (SUCCESS);
}

int	do_expand_cut_helper(t_data *data, char **old, char *s, char i)
{
	char	*tmp;
	t_env	*found;

	tmp = ft_strndup(&(s[1]), i - 1);
	if (!tmp)
		return (set_exit(MAL_FAIL) * -1);
	found = env_find(tmp, data->env);
	free(tmp);
	if (!found || !found->value)
		return (i);
	if (str_join_or_dup(old, found->value, 1))
		return (g_exit);
	return (i);
}

int	do_expand_status(t_data *data, char **old, int *index)
{
	char	*value;

	*index += 2;
	value = ft_itoa(data->status);
	if (!value)
		return (set_exit(MAL_FAIL));
	if (str_join_or_dup(old, value, 3))
		return (set_exit(MAL_FAIL));
	return (SUCCESS);
}

int	do_normal_cut(char *s, char **old, int *index, int quote)
{
	char	*tmp;
	int		i;

	i = 0;
	while (s[i] && s[i] != '$')
	{
		if ((!quote && (s[i] == '\'' || s[i] == '\"')) ||
			(quote && s[i] == quote))
			break;
		i++;
	}
	tmp = ft_strndup(s, i);
	if (!tmp)
		return (set_exit(MAL_FAIL));
	*index += i;
	if (str_join_or_dup(old, tmp, 1))
		return (free(tmp), g_exit);
	free(tmp);
	return (SUCCESS);
}

int	do_expand_cut(t_data *data, char *s, char **old, int *index)
{
	int		i;

	if (!s || *s != '$')
		return (FAIL);
	i = 1;
	if (!ft_isalpha(s[i]) && s[i] != '_')
	{
		if (s[i] == '?' && do_expand_status(data, old, index))
			return (g_exit);
 		if (s[i] != '?' && do_expand_sign(old, index))
			return (g_exit);
		return (SUCCESS);
	}
	while (s[i] && (ft_isdigit(s[i]) || ft_isalpha(s[i]) || s[i] == '_'))
		i++;
	i = do_expand_cut_helper(data, old, s, i);
	if (i < 0)
		return (MAL_FAIL);
	*index += i;
	return (SUCCESS);
}

int	do_double_quote_cut(t_data *data, char *arg, char **old, int *index)
{
	int	i;

	i = 1;
	while (arg[i] && arg[i] != '\"')
	{
		if (arg[i] == '$')
		{
			if (do_expand_cut(data, &(arg[i]), old, &i))
				return (g_exit);
		}
		else if (arg[i] != '\"')
		{
			if (do_normal_cut(&(arg[i]), old, &i, '\"'))
				return (g_exit);
		}
	}
	if (!arg[i])
		return (FAIL); // mismatching quotes error
	*index += i + 1;
	return (SUCCESS);
}

int	do_expand_arg(t_data *data, char **new, char *arg)
{
	int quo;
	int	i;

	quo = 0;
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '\'')
			quo = !quo;
		if (arg[i] == '\'' && quo && do_quote_cut(&(arg[i]), new, &quo, &i))
			return (g_exit);
		else if (arg[i] == '$' && do_expand_cut(data, &(arg[i]), new, &i))
			return (g_exit);
		else if (arg[i] == '\"' && do_double_quote_cut(data, &(arg[i]), new, &i))
			return (g_exit);
		else if (do_normal_cut(&(arg[i]), new, &i, 0))
			return (g_exit);
	}
	return (SUCCESS);
}

int	do_command_arg_add(t_data *data, t_command *cur, char *arg)
{
	char	*tmp;

	tmp = NULL;
	if (do_expand_arg(data, &tmp, arg))
		return (free(tmp), set_exit(MAL_FAIL));
	if (!*tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	if (tmp && token_add(&(cur->args), tmp))
		return (free(tmp), set_exit(MAL_FAIL));
	return (SUCCESS);
}

int	do_command_split(t_data *data, char **tokens)
{
	t_command	*cur;
	int			i;

	cur = data->cmd;
	i = 0;
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "<") || !ft_strcmp(tokens[i], ">") ||
			!ft_strcmp(tokens[i], "<<") || !ft_strcmp(tokens[i], ">>"))
		{
			if (do_add_file(cur, tokens, i++))
				return (g_exit);
		}
		else if (!ft_strcmp(tokens[i], "|"))
		{
			if (do_command_jump(data, &cur, tokens[i + 1]))
				return (g_exit);
		}
		else if (do_command_arg_add(data, cur, tokens[i]))
			return (g_exit);
		i++;
	}
	return (SUCCESS);
}

t_command	*get_command_last(t_command *head)
{
	t_command	*cur;

	if (!head)
		return (NULL);
	cur = head;
	while (cur->next)
		cur = cur->next;
	return (cur);
}

int	do_heredoc_read(t_data *data)
{
	t_command	*tmp_command;
	t_file		*tmp_file;

	tmp_command = data->cmd;
	while (tmp_command)
	{
		tmp_file = tmp_command->files;
		while (tmp_file)
		{
			if (tmp_file->type == Here_doc)
			{
				tmp_file->delim = tmp_file->name;
				tmp_file->name = ft_itoa(tmp_file->index);
				if (!tmp_file->name)
					return (set_exit(MAL_FAIL));
				tmp_file->name = ft_strjoin_free(HERE_DOC_NAME, tmp_file->name, 2);
				if (!tmp_file->name)
					return (set_exit(MAL_FAIL));
				if (do_heredoc(data, tmp_file))
					return (g_exit);
			}
			tmp_file = tmp_file->next;
		}
		tmp_command = tmp_command->next;
	}
	return (SUCCESS);
}

int	input_parse(t_data *data, char **tokens)
{
	if (!tokens)
		return (SUCCESS);
	data->cmd = ft_calloc(1, sizeof(t_command));
	if (!(data->cmd))
		return (set_exit(MAL_FAIL));
	data->cmd->_data = data;
	if (do_command_split(data, tokens))
		return (g_exit);
	if (do_heredoc_read(data))
		return (g_exit);
	return (SUCCESS);
}

int	input_process(t_data *data)
{
	char	**tokens;
	char	*token;
	int		needle;
	int		i;

	if (!data->line)
		return (SUCCESS);
	i = 0;
	tokens = NULL;
	needle = 0;
	while(data->line[i])
	{
		token = tokenizer(data, &i, &needle);
		if (!token)
			return (do_clean_args(tokens), set_exit(MAL_FAIL));
		if (token_add(&tokens, token))
			return (do_clean_args(tokens), free(token), g_exit);
	}
	if (input_parse(data, tokens))
		return (do_clean_args(tokens), g_exit);
	do_clean_args(tokens);
	return (SUCCESS);
}

int get_input(t_data *data)
{
	if (input_loop(data) || data->end)
		return (g_exit);
	add_history(data->line);
	if (input_process(data))
		return (g_exit);
	free(data->line);
	data->line = NULL;
	return (SUCCESS);
}

int main(int ac, char **av, char **envp)
{
	t_data data;

	(void)ac;
	(void)av;
	init_data(&data);
	if (envp_to_env(envp, &data))
		return (do_clean_data(&data), g_exit);
	if (env_update(&data))
		return (do_clean_data(&data), g_exit);
	if (env_to_envp(&data))
		return (do_clean_data(&data), g_exit);
	while (!data.end)
	{
		do_sig(MAIN);
		data.status = g_exit;
		g_exit = 0;
		if (get_input(&data) || data.end)
			break ;
		if (!data.end && !g_exit)
			g_exit = executor(&data);
		do_clean_commands(&data);
	}
	do_clean_data(&data);
	return (data.status % 256);
}
