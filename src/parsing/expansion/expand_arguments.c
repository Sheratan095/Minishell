/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arguments.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 22:30:21 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define DOLLAR 36
#define DOUBLE_QUOTE 34
#define SINGLE_QUOTE 39

//Included by expand_part.c
char		*expand_single_part(t_mshell *mshell, char	*arg_part);
//Included by scan_for_redirections.c
char		*expand_single_arg(t_mshell *mshell, char *curr_arg);

static char	*hanlde_quoted_part(t_mshell *mshell, char *arg, int start, int *i);

/**
 @brief Expand all arguments and remove quotes from quoted parts

 @param mshell Pointer to main structure
 @param command Pointer to the command

 @details
	For each argument
		expand it
		free old arg
		Assign the new arg

 @return Matrix containing expanded arguments
*/
char	**expand_args(t_mshell *mshell, t_command *command)
{
	int		i;
	char	*rewritten_arg;

	i = 0;
	while (command->arguments[i])
	{
		rewritten_arg = expand_single_arg(mshell, command->arguments[i]);
		free(command->arguments[i]);
		command->arguments[i] = rewritten_arg;
		i++;
	}
	return (command->arguments);
}

/**
 @brief Divide the argument in parts ([ciao]["come"]['stai'])
			and expand a portion at time

 @param mshell Pointer to main structure
 @param arg Pointer to string containing the argument

 @details
	Pass through the arg
		if ther's a quote
			Handle quoted part
		if ther's something else (not quote)
			Reach the end of not quoted part
			Get the part avoiding quotes (substr)
			Expand that portion of string
		Add expandend part to new argument

 @note
	expanded_part is just used for normi
 
 @return Rewritten argument
*/
char	*expand_single_arg(t_mshell *mshell, char *arg)
{
	int		start;
	int		i;
	char	*new_arg;
	char	*expanded_part;

	i = 0;
	new_arg = NULL;
	while (arg[i])
	{
		start = i;
		if (arg[i] == SINGLE_QUOTE || arg[i] == DOUBLE_QUOTE)
			expanded_part = hanlde_quoted_part(mshell, arg, start, &i);
		else
		{
			while (arg[i] && arg[i] != SINGLE_QUOTE && arg[i] != DOUBLE_QUOTE)
				i++;
			expanded_part = ft_substr(arg, start, i - start);
			expanded_part = expand_single_part(mshell, expanded_part);
		}
		new_arg = ft_strconcat(new_arg, expanded_part, true, true);
	}
	return (new_arg);
}

/**
 @brief Divide the argument in parts ([ciao]["come"]['stai'])
			and expand a portion at time

 @param mshell Pointer to main structure
 @param arg Pointer to string containing the argument
 @param start Index where the quoted part start
 @param i Pointer to i

 @details
	Pass through the arg
		Skip opening quote
		Reach the end of quoted part
		if the quotes are single
			Get the quoted part avoiding quotes
		if the quotes are double
			Get the quoted part avoiding quotes
			Expand the quoted part
		Skip closing quote	

 @return Expanded part
*/
static char	*hanlde_quoted_part(t_mshell *mshell, char *arg, int start, int *i)
{
	char	*expanded_part;

	expanded_part = NULL;
	(*i)++;
	while (arg[*i] && arg[*i] != arg[start])
		(*i)++;
	if (arg[start] == SINGLE_QUOTE)
		expanded_part = ft_substr(arg, start + 1, *i - start - 1);
	else if (arg[start] == DOUBLE_QUOTE)
	{
		expanded_part = ft_substr(arg, start + 1, *i - start - 1);
		expanded_part = expand_single_part(mshell, expanded_part);
	}
	(*i)++;
	return (expanded_part);
}
