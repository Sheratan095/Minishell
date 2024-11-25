/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 18:03:43 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sigint_received;

t_command		**get_commands(t_mshell *mshell, char *line);
static char		*hanlde_read(t_mshell *mshell);
static t_bool	manage_input(t_mshell *mshell, char *line);
static void		signal_handler(int signal);

//exit_code is used to keep in memory the last exit status after mshell is freed
int	main(int argc, char **argv, char **envp)
{
	int			exit_code;
	char		*current_command_line;
	t_mshell	*mshell;

	mshell = initialize_shell(argc, argv, envp);
	while (true)
	{
		signal(SIGINT, signal_handler);
		g_sigint_received = false;
		current_command_line = hanlde_read(mshell);
		if (current_command_line == NULL)
			break ;
		if (!ft_is_string_empty(current_command_line))
		{
			if (manage_input(mshell, current_command_line) == false)
			{
				free(current_command_line);
				break ;
			}
			free(current_command_line);
		}
	}
	exit_code = mshell->exit_code;
	free_mshell(mshell);
	exit (exit_code);
}

static char	*hanlde_read(t_mshell *mshell)
{
	char	*current_command_line;

	current_command_line = readline(PROMPT);
	add_history(current_command_line);
	mshell->garbage->main_cmd_line = current_command_line;
	return (current_command_line);
}

static t_bool	manage_input(t_mshell *mshell, char *line)
{
	t_command	**commands;
	int			commands_count;
	t_bool		result;

	commands = get_commands(mshell, line);
	if (!commands)
		return (true);
	commands_count = ft_count_matrix_row((const void **)commands);
	if (commands_count == 1)
	{
		commands[0]->indx = 1;
		commands[0]->lastx = 1;
		result = execute_single_command(mshell, commands[0]);
	}
	else
		result = exec_pipeline(commands, commands_count, mshell, 0);
	ft_free_cmd_matrix(commands);
	return (result);
}

//ctrl+c
// Move to a new line
// Tell readline to start on a new line
// Clear the current input line
// Redisplay the prompt
static void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_sigint_received = true;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
