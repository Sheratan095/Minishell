/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:42:00 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 @brief Prints the current working directory.
 
 @param command Pointer to command
 
 @details
 	Determine the size of the buffer required to hold the current directory path
	=> pathconf is used to get configuration information at runtime about
		a specific file or directory (".")
	=> _PC_PATH_MAX is a constant used with the pathconf that determine
		the max length of a relative pathname

	Allocate result string (cwd)

	Getcwd() store the current working directory in cwd pointer untill
		size characters

	Check the result

 @note we haven't to handle option
	original pwd doesn't admits args but it doesn't print error

 @retval 0 (SUCCESFULL) if everything went well
 @retval 12 (ALLOCATION_FAILED) if something went wrong with allocation
*/
int	pwd(t_command *command)
{
	char	*cwd;
	long	size;

	command->mshell->last_exit_status = 0;
	size = pathconf(".", _PC_PATH_MAX);
	cwd = (char *)malloc((size_t)size);
	if (!cwd)
		return (print_error("Memory allocation error"), ALLOCATION_FAILED);
	if (getcwd(cwd, (size_t)size) != NULL)
		printf("%s\n", cwd);
	else
		print_error("getcwd() error");
	free(cwd);
	return (SUCCESSFUL);
}
