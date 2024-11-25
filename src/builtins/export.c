/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 19:09:31 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		export_var(t_mshell *mshell, char *argument);
static char		**split_arg(char *arg, t_bool *append);
static t_bool	is_format_valid(char *arg);
static void		export_whitout_args(t_mshell *mshell);

/**
 @brief Create new var and add it to the list

 @param command Pointer to command

 @details
	Check if the arg has args
	Scroll through the list of args
		if the current arg is well formatted
			try to export that argument
			the result will be SUCCESSFUL
		else
			the result will be GENERIC_ERROR
		Go on

 @return exit code based on validity of last argument
 @retval 0 (SUCCESFULL) if everything went well
 @retval 1 (GENERIC_ERROR) if the format of an arg isn-t valid
*/
int	export(t_command *command)
{
	int	i;
	int	exit_code;

	exit_code = SUCCESSFUL;
	if (get_command_arg_count(command) == 0)
		return (export_whitout_args(command->mshell), SUCCESSFUL);
	i = 0;
	while (command->arguments[i])
	{
		if (is_format_valid(command->arguments[i]))
		{
			export_var(command->mshell, command->arguments[i]);
			exit_code = SUCCESSFUL;
		}
		else
			exit_code = GENERIC_ERROR;
		i++;
	}
	return (exit_code);
}

/**
 @brief Create new var and add it to the list

 @param mshell Pointer to main struct
 @param argument Pointer to the argument

 @details
	Split the arument
	Get the var by key
	if ther's += and exist both var and var->value
		concat the old value with the new one
	if exist a var with the same name of the new one
		Update value of that var
	else
		Create a new var
	free the splitted arg
*/
static void	export_var(t_mshell *mshell, char *argument)
{
	char		**splitted;
	t_bool		append;
	t_env_var	*var;

	append = false;
	splitted = split_arg(argument, &append);
	var = get_var_by_key(mshell, splitted[0]);
	if (append && var && var->value)
		splitted[1] = ft_strconcat(var->value, splitted[1], 0, 1);
	if (var)
		update_var(var, splitted[1]);
	else
		new_var(mshell, splitted[0], splitted[1], false);
	ft_free_matrix((void **)splitted);
}

/**
 @brief Manually split the arg to preserve all other '='
 
		Splitting with 'ft_split' all the '=' would be skipped
		
		But the value of the new env var could contain it

 @param arg Pointer to the arg to split

 @details
	Allocate new matrix [key=splitted[0]; value=splitted[1]]
	Memcheck
	Retrieve the index of first '='
	if before the '=' ther's '+'
		=> the new value has to be appended to the old value
		the key reach the index of equal - 1 (index of '+')
	else
		the key start from 0 and reach the index of equal
	Get the new value and the new string	
	Memcheck for both key and value

 @retval The splitted arg
*/
static char	**split_arg(char *arg, t_bool *append)
{
	char	**splitted;
	int		index_of_equal;
	int		arg_len;

	splitted = ft_calloc(3, sizeof(char *));
	if (!splitted)
		return (NULL);
	arg_len = ft_strlen(arg);
	index_of_equal = ft_str_idxofchar(arg, '=');
	if (arg[index_of_equal - 1] && arg[index_of_equal - 1] == '+')
	{
		*append = true;
		splitted[0] = ft_substr(arg, 0, index_of_equal - 1);
	}
	else
		splitted[0] = ft_substr(arg, 0, index_of_equal);
	splitted[1] = ft_substr(arg, index_of_equal + 1, arg_len - index_of_equal);
	if (!splitted[0] || !splitted[1])
		return (NULL);
	return (splitted);
}

/**
 @brief Check if the new var has a valid format

 @param arg Pointer to a string containing the new var

 @details
	Reach the '=' (end of key)
		if ther's '+' and then ther's '='
			Go on only if it isn't at the start of arg
		if	first char is a digit or
			the current char it's neither a character nor '_' or
			the current char is a space
			=> error
	Check that '=' has been reached
		it shouldn't print error (bash don't do it)

 @note
	There may be other '=' they will included in the value
	In bash key could contains also numbers

	THIS IF IT JUST FOR NORMI
	if ((i == 0 && ft_isdigit(arg[i]))
	|| (!ft_isalnum(arg[i]) && arg[i] != '_')
	|| ft_isspace(arg[i]))
	return (print_invalid_id(ft_substr(arg, 0, ft_strlen(arg)),
			true), false);

 @retval True if the format is valid
 @retval False if something is wrong 
*/
static t_bool	is_format_valid(char *arg)
{
	int		i;

	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == '+' && arg[i + 1] && arg[i + 1] == '=')
		{
			if (i == 0)
				return (print_invalid_id(ft_substr(arg, 0, ft_strlen(arg)), 1),
					0);
			i++;
			continue ;
		}
		if ((i == 0 && !ft_isalpha(arg[0]) && arg[0] != '_')
			|| (i != 0 && !ft_isalnum(arg[i]) && arg[i] != '_'))
			return (print_invalid_id(ft_substr(arg, 0, ft_strlen(arg)), 1), 0);
		i++;
	}
	if (i == 0)
		return (print_invalid_id(ft_substr(arg, 0, ft_strlen(arg)), true), 0);
	if (arg[i] != '=')
		return (false);
	return (true);
}

/**
 @brief Print the just vars marked for export

 @param mshell Pointer to the main struct

 @details
	Just like env but with 'declare -x' in front of the var
	
 @note
	Unlike env() that lists all current environment
		variables (whether marked for export or not).
	export prints just the vars marked for export
		(it means that the variable  is available not only to
			the current shell, but also to any child processes)
*/
static void	export_whitout_args(t_mshell *mshell)
{
	t_env_var	*tmp;

	tmp = mshell->env_vars->next;
	while (tmp)
	{
		printf("declare -x ");
		print_env_var(tmp);
		tmp = tmp->next;
	}
}
