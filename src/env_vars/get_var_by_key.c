/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_by_key.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 21:27:21 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_vars.h"

/**
 @brief Retrieves the environment variable with the specified name

 @param mshell A pointer to a main structure
 @param target_name Name of the var to retrieve

 @details
	Searches through the list of environment variables
		variable whose name matches `target_name`.
	The search is case-sensitive and matches only exact names.
	If the fouonded is the first var in the list ($?)
		free the precedent string (NULL)
		update its value with tostring() of last exit code

 @retval Founded variable
 @retval Null if none of variables matches the name
*/
t_env_var	*get_var_by_key(t_mshell *mshell, char *target_name)
{
	t_env_var	*tmp;

	tmp = mshell->env_vars;
	while (tmp)
	{
		if (ft_strlen(tmp->key) == ft_strlen(target_name)
			&& ft_strncmp(tmp->key, target_name, ft_strlen(target_name)) == 0)
		{
			if (tmp == mshell->env_vars)
			{
				free(tmp->value);
				if (g_sigint_received)
					tmp->value = ft_itoa(130);
				else
					tmp->value = ft_itoa(mshell->last_exit_status);
			}
			return (tmp);
		}
		tmp = tmp->next;
	}
	return (NULL);
}
