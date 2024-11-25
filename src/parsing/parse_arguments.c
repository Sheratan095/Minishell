/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 19:08:37 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define DOUBLE_QUOTE 34
#define SINGLE_QUOTE 39

static size_t		get_arg_count(char **splitted_cmd);
static char			**get_args(t_command *command, char **splitted_cmd);
static t_bool		contains_unclosed_quotes(char *arg);

/**
 @brief Parse the arguments form input string

 @param command Pointer to the command
 @param splitted_cmd Pointer to matrix containing the splitte command

 @details
	Check number of arguments
		if it's 0 => no arguments, just return the new_command
		if it's -1 => unclosed quotes, command isn't valid anymore
			=> error, free(command)
	Allocation and memcheck
	Retrieve the arguments

 @note In Bash, empty arguments are managed in a way that preserves
 		their presence in a command's argument list, even though they
		don't contain any characters

 @retval Command updated
 @retval NULL if something went wrong with allocation
			or there're some unclosed quotes
*/
t_command	*parse_arguments(t_command *command, char **splitted_cmd)
{
	int	arg_count;

	arg_count = get_arg_count(splitted_cmd);
	if (arg_count == 0)
		return (command->arguments = NULL, command);
	if (arg_count == -1)
		return (command->syntax_error = true, print_syntax_error(), command);
	command->arguments = ft_calloc(arg_count + 1, sizeof(char *));
	if (!command->arguments)
		return (free_command(command), NULL);
	command->arguments = get_args(command, splitted_cmd);
	return (command);
}

/**
 @brief Gets the number or arguments

 @param splitted_cmd Pointer to matrix containing the splitte command

 @details
	i start from 1 to avoid the main command (splitted[0])
	new_arg is just used to keep trace of the current part of command
		we're working on
	Concatenates one part of the string at a time to the current_part
		until new_arg don't contains not closed quotes or the arg is ended

 @note
	It doesn't concatenates the space as get_args() just because it's not
		relevant for this part, not having it makes the algorithm
		more efficient

 @retval Number of arguments
 @retval -1 if the line contains unclosed quotes
*/
static size_t	get_arg_count(char **splitted_cmd)
{
	int		i;
	int		count;
	char	*new_arg;

	i = 0;
	count = 0;
	while (splitted_cmd[i])
	{
		new_arg = ft_strdup(splitted_cmd[i]);
		while (splitted_cmd[++i] && contains_unclosed_quotes(new_arg))
			new_arg = ft_strconcat(new_arg, splitted_cmd[i], true, false);
		count++;
		if (contains_unclosed_quotes(new_arg))
			return (free(new_arg), -1);
		free(new_arg);
	}
	return (count);
}

/**
 @brief Populate the matrix with the arguments

 @param command Pointer to the command
 @param splitted_cmd Pointer to matrix containing the splitte command

 @details
	space[2] it's just a static way to declare a string containing just ' '
		because ft_strconcat require two pointers, so it's the only to
		contatenate a single char
	i start from 1 to avoid the main command (splitted[0])
	Concatenates one part of the string at a time to the current_part
		adding space between them until new_arg don't contain
		not closed quotes or the arg is ended
	When a new arg is ended, assign it to the matrix and free it

 @return Matrix full of arguments
*/
static char	**get_args(t_command *command, char **splitted_cmd)
{
	int		i;
	int		index;
	char	*new_arg;
	char	space[2];

	i = 0;
	index = 0;
	space[0] = ' ';
	space[1] = '\0';
	while (splitted_cmd[i])
	{
		new_arg = ft_strdup(splitted_cmd[i]);
		while (splitted_cmd[++i] && contains_unclosed_quotes(new_arg))
		{
			new_arg = ft_strconcat(new_arg, space, true, false);
			new_arg = ft_strconcat(new_arg, splitted_cmd[i], true, false);
		}
		command->arguments[index++] = ft_strdup(new_arg);
		free(new_arg);
	}
	return (command->arguments);
}

/**
 @brief Check it the arg contains unclosed quotes

 @param arg Pointer to the arg to check

 @details
	Pass through the arg
	if ther's a single quote && weren't already on a double quotes
		=> change the value of the control var for single quote
	// Same thing for double quotes

 @note
	return if true if one of the check vars is true
		so if one of those quotes is still open

 @retval TRUE if the string contains uncloded quotes
 @retval TRUE if the string doesn't contain uncloded quotes
*/
static t_bool	contains_unclosed_quotes(char *arg)
{
	int		i;
	t_bool	in_single_quotes;
	t_bool	in_double_quotes;

	i = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	while (arg[i])
	{
		if (arg[i] == SINGLE_QUOTE && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		if (arg[i] == DOUBLE_QUOTE && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		i++;
	}
	return (in_single_quotes || in_double_quotes);
}
