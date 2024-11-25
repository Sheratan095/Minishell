/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 19:28:41 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 @brief Free the command

 @param command Pointer to command

 @details
	if the command doesn't exists
		return
	if the main command exist
		free it
	if there are any arguments
		free them
	free command
 */
void	free_command(t_command	*command)
{
	if (!command)
		return ;
	if (command->command)
		free(command->command);
	if (command->arguments)
		ft_free_matrix((void **)command->arguments);
	if (command->path)
		free(command->path);
	if (command->fd_in != -1)
		close(command->fd_in);
	if (command->fd_out != -1)
		close(command->fd_out);
	free(command);
}

void	ft_free_cmd_matrix(t_command **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		free_command(matrix[i]);
		i++;
	}
	free(matrix);
}

/**
 @brief Check the match between the command to execute and the command
			that are trying to execute

 @param command Pointer to command
 @param target Name of the dommand that are trying to execute

 @details
	Check the extistance of both parameters
	Check the match of the lengths
	Check the match of the characters
 */
t_bool	is_command(t_command *command, char *target)
{
	if (!command || !target)
		return (false);
	if (ft_strlen(command->command) != ft_strlen(target))
		return (false);
	if (ft_strncmp(target, command->command, ft_strlen(target)) != 0)
		return (false);
	return (true);
}

size_t	get_command_arg_count(t_command	*command)
{
	return (ft_count_matrix_row((const void **)command->arguments));
}
