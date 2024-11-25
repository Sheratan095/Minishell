/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_for_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 17:23:44 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Included form heredoc.c
int				here_doc(t_command *command, char *delimiter);
static char		*get_file_name(t_command *command, int i, size_t symbol_len);
static t_bool	try_get_fd(t_command *command, char *file, char *symbol);
static void		initialize_symbols(char *symbols[]);
static t_bool	redirect(t_command *command, int i, char *symbol);

//i; Argument index
//j; Symbol index
// Check if the argument starts with a redirection symbol
// (i--) Adjust `i` after removal to avoid skipping the next argument
t_bool	scan_for_redirections(t_command *command)
{
	int		i;
	int		j;
	int		symbol_len;
	char	*symbols[5];

	initialize_symbols(symbols);
	i = 0;
	while (command->arguments[i])
	{
		j = 0;
		while (symbols[j])
		{
			symbol_len = ft_strlen(symbols[j]);
			if (ft_strncmp(command->arguments[i], symbols[j], symbol_len) == 0)
			{
				if (redirect(command, i, symbols[j]) == false)
					return (false);
				i--;
				break ;
			}
			j++;
		}
		i++;
	}
	return (true);
}

static t_bool	redirect(t_command *command, int i, char *symbol)
{
	int		symbol_len;
	char	*file;

	symbol_len = ft_strlen(symbol);
	file = NULL;
	file = get_file_name(command, i, symbol_len);
	if (!file)
		return (print_syntax_error(), false);
	if (file[0] == '<' || file[0] == '>')
	{
		print_error("minishell: syntax error near unexpected token `newline'");
		free(file);
		return (false);
	}
	if (!try_get_fd(command, file, symbol))
		return (free(file), false);
	free(file);
	return (true);
}

// Check if the symbol and file name are combined in one argument
// This function removes the symbol and file name argument from arguments
static char	*get_file_name(t_command *command, int i, size_t symbol_len)
{
	int		start_i;
	int		arg_len;
	char	*result;

	start_i = i;
	result = NULL;
	if (ft_strlen(command->arguments[i]) > symbol_len)
	{
		arg_len = ft_strlen(command->arguments[i]);
		result = ft_substr(command->arguments[i],
				symbol_len, arg_len - symbol_len);
	}
	else
	{
		i++;
		if (command->arguments[i])
		{
			result = ft_strdup(command->arguments[i]);
			command->arguments = ft_matrix_remove_at(command->arguments, i, 1);
		}
	}
	command->arguments = ft_matrix_remove_at(command->arguments, start_i, true);
	return (result);
}

static t_bool	try_get_fd(t_command *command, char *file, char *symbol)
{
	if (ft_strncmp(symbol, ">>", 2) == 0)
	{
		if (open_file(file, O_WRONLY | O_CREAT | O_APPEND, &command->fd_out)
			== -1)
			return (command->mshell->last_exit_status = 1, false);
	}
	else if (ft_strncmp(symbol, "<<", 2) == 0)
	{
		if (here_doc(command, file) == false)
			return (command->mshell->last_exit_status = 130, false);
		if (open_file(HEREDOC_FILE, O_RDONLY, &command->fd_in) == -1)
			return (false);
	}
	else if (ft_strncmp(symbol, ">", 1) == 0)
	{
		if (open_file(file, O_WRONLY | O_CREAT | O_TRUNC, &command->fd_out)
			== -1)
			return (command->mshell->last_exit_status = 1, false);
	}
	else if (ft_strncmp(symbol, "<", 1) == 0)
	{
		if (open_file(file, O_RDONLY, &command->fd_in) == -1)
			return (command->mshell->last_exit_status = 1, false);
	}
	return (true);
}

//cause i can't initialize the vector in 2 different lines
static void	initialize_symbols(char *symbols[])
{
	symbols[0] = ">>";
	symbols[1] = "<<";
	symbols[2] = ">";
	symbols[3] = "<";
	symbols[4] = NULL;
}
