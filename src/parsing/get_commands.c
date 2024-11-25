/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:44:10 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define DOUBLE_QUOTE 34
#define SINGLE_QUOTE 39
#define PIPE '|'

int					init_vars(int *b, int *a, t_bool *b_one, t_bool *b_two);
t_command			*new_command(t_mshell *mshell, char *str_command);
static ssize_t		get_commands_number(char *line);
static t_bool		parse_cmds(t_mshell *mshell, t_command **cmds, char *line);
static t_command	*get_cmd(t_mshell *mshell, char *line, int i, int start);
static t_bool		handle_quotes(char c_char, t_bool *sin_q, t_bool *doub_q);

t_command	**get_commands(t_mshell *mshell, char *line)
{
	int			command_count;
	char		*trimmed_line;
	t_command	**commands;

	commands = NULL;
	trimmed_line = ft_strtrim(line, " ", false);
	command_count = get_commands_number(line);
	free(trimmed_line);
	if (command_count == -1)
		return (NULL);
	commands = ft_calloc((command_count + 1), sizeof(t_command *));
	mshell->garbage->commands_pipeline = commands;
	if (command_count == 1)
	{
		commands[0] = new_command(mshell, line);
		if (!commands[0])
			return (ft_free_cmd_matrix(commands), NULL);
	}
	else
	{
		if (!parse_cmds(mshell, commands, line))
			return (NULL);
	}
	return (commands);
}

static ssize_t	get_commands_number(char *line)
{
	int		i;
	int		start;
	t_bool	in_single_quotes;
	t_bool	in_double_quotes;
	ssize_t	count;

	start = 0;
	i = 0;
	count = 1;
	in_single_quotes = false;
	in_double_quotes = false;
	while (line[i])
	{
		if (handle_quotes(line[i], &in_single_quotes, &in_double_quotes))
		{
			if (start == i)
				return (print_syntax_error(), -1);
			count++;
			start = i + 1;
		}
		i++;
	}
	if (start == i)
		return (print_syntax_error(), -1);
	return (count);
}

//start = start of new command substring
static t_bool	parse_cmds(t_mshell *mshell, t_command **cmds, char *line)
{
	int		i;
	int		command_index;
	int		start;
	t_bool	in_single_quotes;
	t_bool	in_double_quotes;

	i = init_vars(&command_index, &start, &in_single_quotes, &in_double_quotes);
	while (line[i])
	{
		if (handle_quotes(line[i], &in_single_quotes, &in_double_quotes))
		{
			cmds[command_index] = get_cmd(mshell, line, i, start);
			if (!cmds[command_index])
				return (ft_free_cmd_matrix(cmds), false);
			start = i + 1;
			command_index++;
		}
		i++;
	}
	if (start == i)
		return (ft_free_cmd_matrix(cmds), print_syntax_error(), false);
	cmds[command_index] = get_cmd(mshell, line, i, start);
	if (!cmds[command_index])
		return (ft_free_cmd_matrix(cmds), false);
	return (true);
}

static t_command	*get_cmd(t_mshell *mshell, char *line, int i, int start)
{
	char		*substring;
	t_command	*command;

	command = NULL;
	substring = ft_substr(line, start, i - start);
	if (!substring)
		return (NULL);
	if (ft_is_string_empty(substring))
		return (free(substring), print_syntax_error(), NULL);
	mshell->garbage->new_cmd_substring = substring;
	command = new_command(mshell, substring);
	if (!command)
		return (free(substring), NULL);
	free(substring);
	mshell->garbage->new_cmd_substring = NULL;
	return (command);
}

static t_bool	handle_quotes(char c_char, t_bool *sin_q, t_bool *doub_q)
{
	if (c_char == SINGLE_QUOTE && !*doub_q)
	{
		*sin_q = !*sin_q;
		return (false);
	}
	if (c_char == DOUBLE_QUOTE && !*sin_q)
	{
		*doub_q = !*doub_q;
		return (false);
	}
	if (c_char == PIPE && !*sin_q && !*doub_q)
	{
		*sin_q = false;
		*sin_q = false;
		return (true);
	}
	return (false);
}
