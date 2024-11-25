/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_env_vars.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:09:28 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*concat_var_with_more_equals(char **splitted);

/**
 @brief Load the system environment vars

 @param mshell Pointer to mshell
 @param default_env_vars Pointer matrix containing the system env vars
 @param i Just for norminette

 @details
	Pass through the matrix of vars
	Split each line of matrix
	splitted_arg is supposed to be like 'key=value'
		=> arg[0]=key, arg[1]=value
	Assign to key its value
	if the length of matrix is == 2 (format is ther normal one)
		if the value is NULL (it's possile to create en empty variable)
			value will be just an empty strig ""
		else
			Assign to value its value
		if the key is 'HOME'
			assign the value to home_path in mshell
	else (format is key=va=lu=e)
		join all the other parts and assign the result to the new value
	Memchek for both key and value
	free the splitted var
	Go on

 @retval mshell
 @retval NULL if strdup() fail to allocate 
 */
t_mshell	*load_env_vars(t_mshell *mshell, char **envp, int i)
{
	char	**splitted;
	char	*new_key;
	char	*new_value;

	while (envp[i])
	{
		splitted = ft_split(envp[i], '=');
		new_key = ft_strdup(splitted[0]);
		if (ft_count_matrix_row((const void **)splitted) == 2)
		{
			if (splitted[1] == NULL)
				new_value = ft_strdup("");
			else
				new_value = ft_strdup(splitted[1]);
		}
		else
			new_value = concat_var_with_more_equals(splitted);
		if (ft_strncmp(new_key, "HOME", 4) == 0)
			mshell->home_path = ft_strdup(new_value);
		new_var(mshell, new_key, new_value, true);
		ft_free_matrix((void **)splitted);
		i++;
	}
	return (mshell);
}

/**
 @brief Join the splitted parts when ther're more '=' in the value of the var

 @param splitted Pointer to matrix containing the splitted arg

 @details
	i start from 1 to avoid considering also the key of the var
	Add to the result the current part of the matrix
	if ther's still something in the matrix (not at the end)
		Add '='
	Go on

 @return new value
 */
static char	*concat_var_with_more_equals(char **splitted)
{
	int		i;
	char	*result;

	i = 1;
	result = NULL;
	while (splitted[i])
	{
		result = ft_strconcat(result, splitted[i], true, false);
		if (splitted[i + 1])
			result = ft_strconcat(result, "=", true, false);
		i++;
	}
	return (result);
}
