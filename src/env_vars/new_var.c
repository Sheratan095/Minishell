/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 21:09:24 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_vars.h"

static t_env_var	*connect_to_list(t_mshell *mshell, t_env_var *new_var);
static t_env_var	*get_last_var(t_env_var *vars);

/**
 @brief Create a new environment variable in the list

 @param mshell A pointer to a main structure
 @param name Name of the new var
 @param value Value to assign to new var
 @param free_src Boolean that specifies whether the variables
	passed as parameters should be freed

 @details
	Check if the name is already assigned to a variable
	Allocation and memcheck
	Duplicate the parameters in struct values
	Memcheck in case of ft_strdup fail
	Free parameters just if free_src is true
	Connect the var to the main list in mshell

 @retval Pointer to new environment variable
 @retval Null if the name already exist or something is wrong during allocation
*/
t_env_var	*new_var(t_mshell *mshell, char *key, char *value, t_bool free_src)
{
	t_env_var	*new_var;

	new_var = malloc(sizeof(t_env_var));
	if (get_var_by_key(mshell, key) || !new_var)
		return (NULL);
	new_var->key = ft_strdup(key);
	if (value == NULL)
		new_var->value = ft_strdup("");
	else
		new_var->value = ft_strdup(value);
	if (!new_var->key || !new_var->value)
		return (free_env_var(new_var), NULL);
	if (free_src)
	{
		free(key);
		free(value);
	}
	new_var = connect_to_list(mshell, new_var);
	return (new_var);
}

/**
 @brief Create new var to the ilst in mshell
 
		Just for normi

 @param mshell A pointer to a main structure
 @param new_var Pointer to the new var

 @details
	Assign NULL to the next pointer
	If the list isn't empty (just for first element
		Connect it to the last element in the list
	Else
		Assign new var to head

 @return Pointer to new environment variable
*/
static t_env_var	*connect_to_list(t_mshell *mshell, t_env_var *new_var)
{
	t_env_var	*last_var;

	new_var->next = NULL;
	if (!mshell->env_vars)
	{
		mshell->env_vars = new_var;
	}
	else
	{
		last_var = get_last_var(mshell->env_vars);
		last_var->next = new_var;
	}
	return (new_var);
}

/**
 @brief Retrieves the last element in the list

 @param mshell Pointer to list of environment variables

 @details
	Scrolls the list until the end of the list

 @retval Pointer to lst environment variable in the list
 @retval Null if the list is empty
*/
static t_env_var	*get_last_var(t_env_var *vars)
{
	t_env_var	*tmp;

	if (!vars)
		return (NULL);
	tmp = vars;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	return (tmp);
}
