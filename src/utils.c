/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 18:00:51 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 20:51:15 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_history(HIST_ENTRY **history)
{
	int	i;

	i = 0;
	if (history == NULL)
	{
		printf("No history available.\n");
		return ;
	}
	while (history[i])
		printf("%s\n", history[i++]->line);
}

void	print_env_var(t_env_var *var)
{
	printf("%s=%s\n", var->key, var->value);
}

void	debug_helper(t_mshell *mshell, t_command *command, int pid)
{
	printf("process:\t%d\t\n", pid);
	printf("%d \tSTDIN\t\n", STDIN_FILENO);
	printf("%d \torig STDIN\t\n", mshell->original_fd_in);
	printf("%d \tcommand in\t\n", command->fd_in);
	printf("%d \tSTDOUT\t\n", STDOUT_FILENO);
	printf("%d \torig STDOUT\t\n", mshell->original_fd_out);
	printf("%d \tcommand out\t\n", command->fd_out);
}

t_bool	is_path(char *command)
{
	return ((ft_str_start_with(command, '.')
			|| ft_str_start_with(command, '/')));
}
