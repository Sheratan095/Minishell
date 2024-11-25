/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:16:15 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:15:29 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Included from expand_part.c
char			*expand_single_part(t_mshell *mshell, char	*part);
static t_bool	heredoc_child(t_command *command, char *delimiter);
static void		close_heredoc(t_command *command, int heredoc_fd);
static void		warning(char *delimiter);
static void		signal_handler(int singnal);

t_bool	here_doc(t_command *command, char *delimiter)
{
	int		fork_pid;
	int		fork_exit_status;

	fork_pid = fork();
	if (fork_pid == 0)
	{
		delimiter = ft_strtrim(delimiter, "\"'", true);
		signal(SIGINT, signal_handler);
		if (heredoc_child(command, delimiter) && g_sigint_received == false)
			warning(delimiter);
		free(delimiter);
		if (g_sigint_received)
			exit(1);
		exit(0);
	}
	else
	{
		waitpid(fork_pid, &fork_exit_status, 0);
	}
	if (fork_exit_status == 256)
		return (false);
	return (true);
}

//return true is cltr + d is pressed
static t_bool	heredoc_child(t_command *command, char *delimiter)
{
	char	*str;
	int		heredoc_fd;
	t_bool	cltr_d_pressed;

	cltr_d_pressed = false;
	if (ft_is_string_empty(delimiter))
		return (false);
	heredoc_fd = open(HEREDOC_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	while (true)
	{
		str = readline(PROMPT_HEREDOC);
		if (ft_strcmp(str, delimiter) == 0 || str == NULL)
		{
			if (str == NULL)
				cltr_d_pressed = true;
			free(str);
			break ;
		}
		str = expand_single_part(command->mshell, str);
		ft_putendl_fd(str, heredoc_fd);
		free(str);
	}
	close_heredoc(command, heredoc_fd);
	return (cltr_d_pressed);
}

static void	signal_handler(int singnal)
{
	if (singnal == SIGINT)
	{
		g_sigint_received = true;
		close(0);
	}
}

static void	close_heredoc(t_command *command, int heredoc_fd)
{
	free_all_resources(command);
	close(heredoc_fd);
}

static void	warning(char *delimiter)
{
	printf("Warning: here-document delimited by end-of-file (wanted `%s')\n",
		delimiter);
}
