/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 20:52:57 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Included from load_env_vars.c
t_mshell			*load_env_vars(t_mshell *mshell, char **envp, int i);
static t_garbage	*init_garbage(t_mshell *mshell);
// static void			signal_handler(int signal);

/**
 @brief Create the main struct

 @param default_env_vars Pointer matrix containing the system env vars

 @details
	malloc and memcheck
	Initialize the last exit status to 0
	Initialize environment vars
	signal(SIGQUIT, SIG_IGN) is used to ingore ctrl+'\'
	if (current_command_line == NULL) is used for ctrl+d
 */
t_mshell	*initialize_shell(int argc, char **argv, char **envp)
{
	t_mshell	*mshell;

	(void)argc;
	(void)argv;
	signal(SIGQUIT, SIG_IGN);
	mshell = malloc(sizeof(t_mshell));
	if (!mshell)
		return (NULL);
	mshell->garbage = init_garbage(mshell);
	if (!mshell->garbage)
		return (free_mshell(mshell), NULL);
	mshell->home_path = NULL;
	mshell->last_exit_status = 0;
	mshell->exit_code = 0;
	mshell->envp = envp;
	mshell->env_vars = NULL;
	mshell->pipeline = NULL;
	mshell->original_fd_in = dup(STDIN_FILENO);
	mshell->original_fd_out = dup(STDOUT_FILENO);
	new_var(mshell, "?", "0", false);
	mshell = load_env_vars(mshell, envp, 0);
	return (mshell);
}

/**
 @brief Free the main struct

 @param mshell Pointer to mshell
 @param default_env_vars Pointer matrix containing the system env vars

 @details
	if the variables exist
		free them
	if ther's home_path
		free it
	free the main struct
 */
void	free_mshell(t_mshell *mshell)
{
	if (mshell->env_vars)
		free_env_vars(mshell);
	if (mshell->home_path)
		free(mshell->home_path);
	if (mshell->garbage)
		free(mshell->garbage);
	close(mshell->original_fd_in);
	close(mshell->original_fd_out);
	free(mshell);
	rl_clear_history();
}

void	free_garbage(t_garbage *garbage)
{
	if (garbage->main_cmd_line)
		free(garbage->main_cmd_line);
	if (garbage->new_cmd_substring)
		free(garbage->new_cmd_substring);
	if (garbage->commands_matrix)
		ft_free_matrix((void **)garbage->commands_matrix);
	if (garbage->commands_pipeline)
		ft_free_cmd_matrix(garbage->commands_pipeline);
}

static t_garbage	*init_garbage(t_mshell *mshell)
{
	mshell->garbage = malloc(sizeof(t_garbage));
	if (!mshell->garbage)
		return (free_mshell(mshell), NULL);
	mshell->garbage->commands_matrix = NULL;
	mshell->garbage->commands_pipeline = NULL;
	mshell->garbage->main_cmd_line = NULL;
	mshell->garbage->new_cmd_substring = NULL;
	return (mshell->garbage);
}
