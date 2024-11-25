/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:58:53 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 @brief Delete and free the specified var from the list

 @param mshell Pointer to a main structure
 @param to_delete Pointer to the var to delete

 @details
	tmp is used to pass through the list
	prec is used to keep in memory the pointer to the precedent var

	Traverse the list until found the specified var
		Connect the precedent and next
		Free that var

 @retval True if the operation was successful
 @retval False if something went wrong
*/
t_bool	delete_env_var(t_mshell	*mshell, t_env_var *to_delete)
{
	t_env_var	*tmp;
	t_env_var	*prec;

	if (!mshell || !to_delete)
		return (false);
	prec = mshell->env_vars;
	tmp = mshell->env_vars->next;
	while (tmp)
	{
		if (tmp == to_delete)
		{
			prec->next = tmp->next;
			free_env_var(tmp);
			return (true);
		}
		prec = tmp;
		tmp = tmp->next;
	}
	return (false);
}
