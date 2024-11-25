/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 19:08:37 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		**expand_args(t_mshell *mshell, t_command *command);
t_bool		scan_for_redirections(t_command *command);
t_command	*parse_arguments(t_command *command, char **splitted_cmd);
t_command	*get_main_command(t_command *new_command);
t_command	*get_new_main_command(t_command *command);
static void	init_command(t_command *new_command, t_mshell *mshell);
static void	close_fds(t_command *command);

/**
 @brief Create new command

 @param mshell Pointer to main structure
 @param str_command Pointer to string containing the command line

 @details
	Allocation and memcheck
	Split str_command
	Get the main command (splitted[0])
	Expand it
	Retrieves the arguments and memcheck
	If the there're some arguments
		expand them
	free(command_splitted)

 @note In Bash, empty arguments are managed in a way that preserves
 		their presence in a command's argument list, even though they
		don't contain any characters
		Error is changed just if it reach the
			end and not every time ther's an error

 @retval Command
 @retval NULL if something went wrong with allocation
			or there're some unclosed quotes
*/
t_command	*new_command(t_mshell *mshell, char *str_command)
{
	t_command	*new_command;
	char		**command_splitted;

	if (ft_is_string_empty(str_command))
		return (NULL);
	new_command = malloc(sizeof(t_command));
	if (!new_command)
		return (NULL);
	init_command(new_command, mshell);
	command_splitted = ft_split(str_command, ' ');
	new_command = parse_arguments(new_command, command_splitted);
	ft_free_matrix((void **)command_splitted);
	new_command = get_main_command(new_command);
	if (get_command_arg_count(new_command) != 0)
	{
		if (!scan_for_redirections(new_command))
			return (new_command->error = true, new_command);
		if (get_command_arg_count(new_command) != 0)
			new_command->arguments = expand_args(mshell, new_command);
		if (get_command_arg_count(new_command) != 0 && !new_command->command)
			new_command = get_new_main_command(new_command);
	}
	if (!new_command->command)
		return (close_fds(new_command), new_command->error = true, new_command);
	return (new_command);
}

static void	init_command(t_command *new_command, t_mshell *mshell)
{
	new_command->error = false;
	new_command->syntax_error = false;
	new_command->indx = 0;
	new_command->command = NULL;
	new_command->arguments = NULL;
	new_command->path = NULL;
	new_command->mshell = mshell;
	new_command->fd_in = -1;
	new_command->fd_out = -1;
}

static void	close_fds(t_command *command)
{
	if (command->fd_in != -1)
		close(command->fd_in);
	command->fd_in = -1;
	if (command->fd_out != -1)
		close(command->fd_out);
	command->fd_out = -1;
}
