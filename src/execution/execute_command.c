/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:34:13 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 20:51:15 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			close_pipe_redirection(t_command *command);
int				redirect(int from, int to);
t_bool			fetch_command(t_command *command);
static void		execute_command(t_command *command, t_mshell *mshell);
static int		run_builtin(t_command *command);
static t_bool	is_command_executable(t_command *command);
static void		signal_hanlder_single_command(int signal);

t_bool	execute_single_command(t_mshell *mshell, t_command *command)
{
	int	tmp_fd_in;
	int	tmp_fd_out;

	if (!is_command_executable(command))
		return (true);
	mshell->last_exit_status = -1;
	tmp_fd_in = -1;
	tmp_fd_out = -1;
	if (command->fd_out != -1)
		tmp_fd_out = redirect(STDOUT_FILENO, command->fd_out);
	if (run_builtin(command) != COMMAND_NOT_FOUND)
	{
		restore_redirection(mshell, tmp_fd_in, tmp_fd_out);
		return ((mshell->last_exit_status != EXIT));
	}
	if (!fetch_command(command))
	{
		restore_redirection(mshell, tmp_fd_in, tmp_fd_out);
		print_command_not_found(command->command);
		return (mshell->last_exit_status = COMMAND_NOT_FOUND, true);
	}
	if (command->fd_in != -1)
		tmp_fd_in = redirect(STDIN_FILENO, command->fd_in);
	execute_command(command, mshell);
	return (restore_redirection(mshell, tmp_fd_in, tmp_fd_out), true);
}

static void	execute_command(t_command *command, t_mshell *mshell)
{
	int		fork_pid;
	int		status;
	char	**args;

	fork_pid = fork();
	signal(SIGINT, signal_hanlder_single_command);
	if (fork_pid == 0)
	{
		args = ft_matrix_add_front(command->arguments, command->command, false);
		if (command->lastx != 1)
			close_pipe_redirection(command);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(command->path, args, env_to_matrix(mshell)) == -1)
		{
			perror("execve error");
			_exit(EXIT_FAILURE);
		}
	}
	else if (command->lastx == 1)
	{
		waitpid(fork_pid, &status, 0);
		mshell->last_exit_status = -1;
		recv_exit_status(status, mshell);
	}
}

static int	run_builtin(t_command *command)
{
	command->mshell->last_exit_status = -1;
	if (is_command(command, "pwd"))
		command->mshell->last_exit_status = pwd(command);
	if (is_command(command, "cd"))
		command->mshell->last_exit_status = cd(command);
	if (is_command(command, "env"))
		command->mshell->last_exit_status = env(command);
	if (is_command(command, "unset"))
		command->mshell->last_exit_status = unset(command);
	if (is_command(command, "export"))
		command->mshell->last_exit_status = export(command);
	if (is_command(command, "echo"))
		command->mshell->last_exit_status = echo(command);
	if (is_command(command, "exit"))
		command->mshell->last_exit_status = exit_mshell(command);
	if (command->mshell->last_exit_status == -1)
		return (COMMAND_NOT_FOUND);
	else
		return (command->mshell->last_exit_status);
}

static t_bool	is_command_executable(t_command *command)
{
	if (command->syntax_error == true)
	{
		command->mshell->last_exit_status = IMPROPER_COMMAND_USAGE;
		return (false);
	}
	if (command->error)
		return (false);
	return (true);
}

//Used to override the main singnal hanlder to don't leet it print the prompt
//	twice when ctrl + c is pressed
static void	signal_hanlder_single_command(int signal)
{
	if (signal == SIGINT)
	{
		g_sigint_received = true;
		ft_printf("\n");
	}
}
