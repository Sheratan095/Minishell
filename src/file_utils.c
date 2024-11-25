/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:40:37 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 20:51:15 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
	{
		perror("stat");
		return (0);
	}
	return (S_ISDIR(statbuf.st_mode));
}

static t_bool	check_main_command(t_command *command)
{
	if ((ft_str_start_with(command->command, '.')
			|| ft_str_start_with(command->command, '/'))
		&& access(command->command, X_OK) != -1
		&& !is_directory(command->command))
	{
		command->path = ft_strdup(command->command);
		return (true);
	}
	return (false);
}

int	open_file(char *file, int mode, int *target)
{
	int	fd;

	fd = open(file, mode, 0644);
	if (fd == -1)
		return (printf(RED "%s: no such file or directory\n"RESET, file), -1);
	if (*target != -1)
		close(*target);
	*target = fd;
	return (fd);
}

void	free_all_resources(t_command *command)
{
	free_garbage(command->mshell->garbage);
	free_mshell(command->mshell);
	free_command(command);
}

t_bool	fetch_command(t_command *command)
{
	int			i;
	char		**mpath;
	char		*cmd;
	t_env_var	*paths;

	if (is_path(command->command))
		return (check_main_command(command));
	cmd = "/";
	cmd = ft_strconcat(cmd, command->command, false, false);
	i = 0;
	paths = get_var_by_key(command->mshell, "PATH");
	if (!paths)
		return (free(cmd), false);
	mpath = ft_split(paths->value, ':');
	while (mpath[i])
	{
		mpath[i] = ft_strconcat(mpath[i], cmd, true, false);
		if (access(mpath[i], F_OK) != -1 && !is_directory(mpath[i]))
			return (command->path = ft_strdup(mpath[i]), free(cmd),
				ft_free_matrix((void **)mpath), true);
		i++;
	}
	return (free(cmd), ft_free_matrix((void **)mpath), false);
}
