/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:42:09 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env_vars.h"

void	print_error(char *message)
{
	ft_printf(RED "%s\n" RESET, message);
}

void	print_command_not_found(char *command)
{
	ft_printf(RED "Error '%s' command not found\n" RESET, command);
}

void	print_invalid_id(char	*identifier, t_bool free_src)
{
	ft_printf(RED "export: `%s`: not a valid identifier\n" RESET, identifier);
	if (free_src)
		free(identifier);
}

void	print_syntax_error(void)
{
	ft_printf(RED "Error, syntax error\n" RESET);
}
