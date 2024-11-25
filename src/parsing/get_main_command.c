/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_main_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 00:26:51 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define DOUBLE_QUOTE 34
#define SINGLE_QUOTE 39

char				*expand_single_part(t_mshell *mshell, char	*arg_part);
char				*expand_single_arg(t_mshell *mshell, char *arg);
static t_command	*hanlde_unquoted_part(t_command *cmd, char *tmp);

t_command	*get_main_command(t_command *cmd)
{
	int		tmp_len;
	char	*tmp;

	if (!cmd->arguments)
		return (cmd);
	tmp = ft_strdup(cmd->arguments[0]);
	tmp_len = ft_strlen(tmp);
	if ((tmp[0] == SINGLE_QUOTE && tmp[tmp_len - 1] == SINGLE_QUOTE)
		|| (tmp[0] == DOUBLE_QUOTE && tmp[tmp_len - 1] == DOUBLE_QUOTE))
	{
		cmd->command = expand_single_arg(cmd->mshell, tmp);
		cmd->arguments = ft_matrix_remove_at(cmd->arguments, 0, true);
	}
	else
		cmd = hanlde_unquoted_part(cmd, tmp);
	free(tmp);
	return (cmd);
}

static t_command	*hanlde_unquoted_part(t_command *cmd, char *tmp)
{
	int		i;
	int		tmp_len;
	char	*substr;

	i = 0;
	tmp_len = ft_strlen(tmp);
	substr = NULL;
	while (tmp[i] && tmp[i] != '<' && tmp[i] != '>')
		i++;
	if (i == 0)
		return (cmd);
	cmd->command = expand_single_part(cmd->mshell, ft_substr(tmp, 0, i));
	cmd->arguments = ft_matrix_remove_at(cmd->arguments, 0, true);
	if (tmp_len > i)
	{
		substr = ft_substr(tmp, i, tmp_len - i);
		cmd->arguments = ft_matrix_add_front(cmd->arguments, substr, true);
		free(substr);
	}
	return (cmd);
}

t_command	*get_new_main_command(t_command *command)
{
	command->command = ft_strdup(command->arguments[0]);
	command->arguments = ft_matrix_remove_at(command->arguments, 0, true);
	return (command);
}
