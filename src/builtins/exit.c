/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:54:58 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_exit_error(char *str);

/**
 @brief Exit from Minishell

 @param command Pointer to command

 @details
	Set last exit status by default to 0
	Get and check the number of arguments
	if count == 0
		just exit
	if ther're more than an args
		print error end return GENERIC_ERROR
	Get the exit code in the fisrt optional
	if it's a number
		new exit status will be the number

 @note
	This is the only function that directly change ghe value of exit status

 @retval -1 (EXIT) the shell proces must terminate
 @retval 2 (GENERIC_ERROR) if ther're too many arguments
*/
int	exit_mshell(t_command *command)
{
	int		count;
	int		exit_code;

	count = get_command_arg_count(command);
	if (count == 0)
		return (EXIT);
	if (count > 1)
		return (print_error("exit: too many arguments"), 2);
	exit_code = ft_atoi(command->arguments[0]);
	if (exit_code)
		command->mshell->exit_code = exit_code;
	else
		print_exit_error(command->arguments[0]);
	return (EXIT);
}

static void	print_exit_error(char *str)
{
	printf(RED "exit: %s: numeric argument required\n" RESET, str);
}
