/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:13:20 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		close_pipeline(int *fd, int j);
int			*init_pipeline_fds(int **fd, int x);
static void	pipe_command(t_command **commands, int i, int *fd, int j);
static void	init_vars(int *j, int *status, int *orig_in, int *orig_out);
static void	wait_and_close_pipeline(int *fd, int *j, int *x, int *status);

t_bool	exec_pipeline(t_command **cmds, int x, t_mshell *mshell, int i)
{
	int	*fd;
	int	j;
	int	status;
	int	orig_in;
	int	orig_out;

	init_vars(&j, &status, &orig_in, &orig_out);
	mshell->pipeline = init_pipeline_fds(&fd, x);
	while (cmds[i])
	{
		cmds[i]->lastx = x;
		pipe_command(cmds, i, fd, j);
		if (cmds[i]->command)
		{
			if (!execute_single_command(mshell, cmds[i]) && (i + 1) == x)
				return (restore_redirection(mshell, orig_in, orig_out),
					close_pipeline(fd, j + 2), free(fd), false);
		}
		i++;
		j += 2;
	}
	wait_and_close_pipeline(fd, &j, &x, &status);
	restore_redirection(mshell, orig_in, orig_out);
	return (recv_exit_status(status, mshell), free(fd), true);
}

static void	pipe_command(t_command **commands, int i, int *fd, int j)
{
	t_mshell	*mshell;
	int			x;
	int			in;
	int			out;

	mshell = commands[i]->mshell;
	in = mshell->original_fd_in;
	out = mshell->original_fd_out;
	x = commands[i]->lastx;
	pipe(fd + (i * 2));
	commands[i]->indx = i;
	if (i != 0 && in != commands[i]->fd_in && commands[i]->fd_in == -1)
		commands[i]->fd_in = fd[j - 2];
	if (i + 1 != x && out != commands[i]->fd_out && commands[i]->fd_out == -1)
		commands[i]->fd_out = fd[j + 1];
}

static void	init_vars(int *j, int *status, int *orig_in, int *orig_out)
{
	*j = 0;
	*status = 0;
	*orig_in = dup(STDIN_FILENO);
	*orig_out = dup(STDOUT_FILENO);
}

static void	wait_and_close_pipeline(int *fd, int *j, int *x, int *status)
{
	close_pipeline(fd, *j);
	while ((*x)--)
		wait(status);
}
