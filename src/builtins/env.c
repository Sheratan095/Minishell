/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 00:37:25 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 @brief Print all the environment variables

 @param command Pointer to command

	Check arg count
	Start to print from head->next to skip the first one ($?)

 @details
	Pass through the list and call print function on each variables

 @retval 0 (SUCCESSFUL) if everything went well
 @retval 2 (GENERIC_ERROR) ther're some arguments
*/
int	env(t_command *command)
{
	t_env_var	*tmp;

	if (get_command_arg_count(command) > 0)
		return (print_error("env: No arguments/options are allowed"), 1);
	tmp = command->mshell->env_vars->next;
	while (tmp)
	{
		print_env_var(tmp);
		tmp = tmp->next;
	}
	return (SUCCESSFUL);
}
