/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:13:37 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_chdir_error(char *path);
static char	*get_path(t_command *command, char *old_pwd);
static char	*get_oldpwd_and_update(t_command *command);
static void	get_pwd_and_update(t_command *command);

/**
 @brief Change the current working directory

 @param command Pointer to the command to be executed

 @details
	if number of args is > 1
		Error, too many arguments
	if number of args is 0 OR number of args is 1 and the arg is just '~'
		new path will be the home of user
		if che new path is null
			generic error (env vars corrisponding to HOME not found)
	else
		new path is argsuments[0]
	Check if the new path is empty (could happen when cd $not_existing_var)
	Try to change dir
	In case of error
		print corresponding error
		return error

 @retval 0 (SUCCESFULL) if everything went well
 @retval 1 (GENERIC_ERROR) if the target dir isn't found
							or ther're too many arguments
							or there ins't HOME var
*/
int	cd(t_command *command)
{
	char		*path;
	char		*oldpwd_value;

	oldpwd_value = get_oldpwd_and_update(command);
	path = get_path(command, oldpwd_value);
	if (!path)
		return (free(oldpwd_value), GENERIC_ERROR);
	if (ft_is_string_empty(path))
		return (0);
	if (chdir(path) == 0)
		return (free(oldpwd_value), get_pwd_and_update(command), SUCCESSFUL);
	free(oldpwd_value);
	print_chdir_error(path);
	return (GENERIC_ERROR);
}

static char	*get_path(t_command *command, char *old_pwd)
{
	int		arg_count;
	char	*path;

	path = NULL;
	arg_count = get_command_arg_count(command);
	if (arg_count > 1)
		return (print_error("cd: too many arguments"), NULL);
	else if (arg_count == 0
		|| (arg_count == 1 && ft_strncmp("~", command->arguments[0], 1) == 0))
	{
		path = command->mshell->home_path;
		if (!path)
			return (NULL);
	}
	else if (arg_count == 0
		|| (arg_count == 1 && ft_strncmp("-", command->arguments[0], 1) == 0))
	{
		path = old_pwd;
		if (!path)
			return (print_error("cd: OLDPWD not set"), NULL);
	}
	else
		path = command->arguments[0];
	return (path);
}

static char	*get_oldpwd_and_update(t_command *command)
{
	char		*current_dir;
	t_env_var	*oldpwd_var;
	char		*oldpwd_value;

	current_dir = getcwd(NULL, 0);
	oldpwd_var = get_var_by_key(command->mshell, "OLDPWD");
	if (!oldpwd_var)
	{
		oldpwd_value = NULL;
		new_var(command->mshell, "OLDPWD", current_dir, false);
	}
	else
	{
		oldpwd_value = ft_strdup(oldpwd_var->value);
		update_var(oldpwd_var, current_dir);
	}
	free(current_dir);
	return (oldpwd_value);
}

/**
 @brief Print the corresponding error in case of failure of chdir

 @param path Pointer to string containing the path

 @details
	"Switch" between possible error and print
		the corresponding message

 @note errno is a global variable that is set by system calls
		to indicate what went wrong in the event of an error
*/
static void	print_chdir_error(char *path)
{
	if (errno == EACCES)
		printf(RED "cd: %s: Nermission denied\n" RESET, path);
	else if (errno == ENAMETOOLONG)
		printf(RED "cd: %s: Nath too long\n" RESET, path);
	else if (errno == ENOTDIR)
		printf(RED "cd: %s: Not a directory\n" RESET, path);
	else
		printf(RED "cd: %s: No such file or directory\n" RESET, path);
}

static void	get_pwd_and_update(t_command *command)
{
	char		*current_dir;
	t_env_var	*pwd_var;

	current_dir = getcwd(NULL, 0);
	pwd_var = get_var_by_key(command->mshell, "PWD");
	if (!pwd_var)
		new_var(command->mshell, "PWD", current_dir, false);
	else
		update_var(pwd_var, current_dir);
	free(current_dir);
}
