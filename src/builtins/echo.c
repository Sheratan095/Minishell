/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:59:18 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 @brief Print the arguments

 @param command Pointer to command

 @details
	Get the number of arguments
	if ther's something
		if the first argument is '-n'
			Will not print new line
			i starts from 1 to avoid to print also '-n'
		print all the arguments
			if isn't the last arg (to avoid last printed space)
				OR the printed arg isn't empty string
					(echo $not_existing_var something)
				=> print space
			Go oo
	if there wasn't the flag '-n'
		print '\n'

 @retval 0 (SUCCESFULL) everything went well
*/
int	echo(t_command *command)
{
	int		i;
	int		arg_count;
	t_bool	new_line;

	arg_count = get_command_arg_count(command);
	new_line = true;
	if (arg_count > 0)
	{
		i = 0;
		if (ft_strcmp("-n", command->arguments[0]) == 0)
		{
			new_line = false;
			i = 1;
		}
		while (command->arguments[i])
		{
			printf("%s", command->arguments[i]);
			if (i != arg_count && ft_strlen(command->arguments[i]) > 0)
				printf(" ");
			i++;
		}
	}
	if (new_line)
		printf("\n");
	return (SUCCESSFUL);
}
