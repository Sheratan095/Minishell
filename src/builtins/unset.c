/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:43:13 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	is_id_valid(char *arg);

/**
 @brief Delete one or more environment vars

 @param command Pointer to the current command

 @details
	i is used to pass through the matrix of args
		(each potential var to delete)
	result is used to keep the count of deleted var and return correctly

	if the command has not args
		Return false
	Traverse the list
		Try to retrieve env var by any arg
		if exist
			if the elimination goes well
				increase the count
		go on

 @note original bash doesn't print an error when
	ther're no arguments

 @retval 1 (SUCCESFULL) if everything went well
*/
int	unset(t_command *command)
{
	int			i;
	t_env_var	*var_tmp;
	size_t		result;
	int			exit_code;

	i = 0;
	result = 0;
	exit_code = SUCCESSFUL;
	while (command->arguments[i])
	{
		if (!is_id_valid(command->arguments[i]))
		{
			i++;
			exit_code = GENERIC_ERROR;
			continue ;
		}
		var_tmp = get_var_by_key(command->mshell, command->arguments[i]);
		if (var_tmp)
		{
			if (delete_env_var(command->mshell, var_tmp))
				result++;
		}
		i++;
	}
	return (exit_code);
}

static t_bool	is_id_valid(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if ((i == 0 && !ft_isalpha(arg[0]) && arg[0] != '_')
			|| (i != 0 && !ft_isalnum(arg[i]) && arg[i] != '_'))
		{
			print_invalid_id(arg, false);
			return (false);
		}
		i++;
	}
	return (true);
}
