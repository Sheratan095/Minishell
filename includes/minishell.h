/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 15:09:55 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 20:51:15 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include "corekit.h"
# include "env_vars.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/ioctl.h>

# include <readline/readline.h>
# include <readline/history.h>

# include <sys/stat.h>
# include <errno.h>

# include <sys/types.h>
# include <sys/wait.h>

# ifndef PROMPT
#  define PROMPT "\033[0m> \033[0m"
# endif

# ifndef PROMPT_HEREDOC
#  define PROMPT_HEREDOC "heredoc> "
# endif

# ifndef HEREDOC_FILE
#  define HEREDOC_FILE ".heredoc"
# endif

# define RESET "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

# define EXIT -42
# define SUCCESSFUL 0
# define GENERIC_ERROR 1
# define IMPROPER_COMMAND_USAGE 2
# define ALLOCATION_FAILED 12
# define COMMAND_NOT_FOUND 127
# define SIG_INT_RECIVED 130

typedef struct s_command	t_command;

extern int					g_sigint_received;

typedef struct s_garbage
{
	char				*main_cmd_line;
	char				*new_cmd_substring;
	char				**commands_matrix;
	t_command			**commands_pipeline;
}	t_garbage;

//env vars is declared as a prototype beacuase it's declared in env_vars.h
typedef struct s_mshell
{
	int					last_exit_status;
	int					exit_code;

	struct s_env_var	*env_vars;
	char				**envp;
	char				*home_path;
	int					*pipeline;
	int					original_fd_in;
	int					original_fd_out;
	t_garbage			*garbage;
}	t_mshell;

typedef struct s_command
{
	char		*command;
	char		**arguments;
	t_bool		error;
	t_bool		syntax_error;

	t_mshell	*mshell;
	char		*path;
	int			indx;
	int			lastx;

	int			fd_in;
	int			fd_out;
	int			prev_in;

}	t_command;

t_bool		is_path(char *command);

t_mshell	*initialize_shell(int argc, char **argv, char **envp);

t_bool		is_command(t_command *command, char *target);

size_t		get_command_arg_count(t_command	*command);

t_bool		execute_single_command(t_mshell *mshell, t_command *command);

t_bool		exec_pipeline(t_command **cmds, int x, t_mshell *mshell, int i);

void		restore_redirection(t_mshell *mshell, int in, int out);

void		recv_exit_status(int status, t_mshell *mshell);

int			open_file(char *file, int mode, int *target);

////---------------------------------CLEANING---------------------------------

void		free_mshell(t_mshell *mshell);

void		free_garbage(t_garbage *garbage);

void		ft_free_cmd_matrix(t_command **matrix);

void		free_command(t_command	*command);

void		free_all_resources(t_command *command);

//---------------------------------BULTINS---------------------------------

int			pwd(t_command *command);

int			cd(t_command *command);

int			env(t_command *command);

int			unset(t_command *command);

int			export(t_command *command);

int			echo(t_command *command);

int			exit_mshell(t_command *command);

//---------------------------------OUTPUT---------------------------------

void		debug_helper(t_mshell *mshell, t_command *command, int pid);

void		print_error(char *message);

void		print_command_not_found(char *command);

void		print_env_var(struct s_env_var *var);

void		print_invalid_id(char	*identifier, t_bool free_src);

void		print_syntax_error(void);

void		ft_print_history(HIST_ENTRY **history);

#endif
