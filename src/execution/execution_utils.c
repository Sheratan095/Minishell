/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 15:29:43 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:09:36 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	recv_exit_status(int status, t_mshell *mshell)
{
	if (mshell->last_exit_status != -1)
		return ;
	if (WIFEXITED(status))
		mshell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mshell->last_exit_status = WTERMSIG(status) + 128;
}

void	close_pipe_redirection(t_command *command)
{
	int	i;

	i = 0;
	while (i < command->indx * 2)
	{
		close (command->mshell->pipeline[i]);
		i++;
	}
}

void	restore_redirection(t_mshell *mshell, int in, int out)
{
	if (in != -1)
	{
		close(mshell->original_fd_in);
		mshell->original_fd_in = dup2(in, STDIN_FILENO);
		close(in);
	}
	if (out != -1)
	{
		close(mshell->original_fd_out);
		mshell->original_fd_out = dup2(out, STDOUT_FILENO);
		close(out);
	}
}

int	redirect(int from, int to)
{
	int	x;

	x = dup(from);
	dup2(to, from);
	close(to);
	return (x);
}
