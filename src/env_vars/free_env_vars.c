/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env_vars.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 21:20:53 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_vars.h"

/**
 @brief Free all the list of evirorments variables

 @param mshell A pointer to a main structure

 @details
	Traverse the list
	Free the single var
	Move to the next node and repeat the process until the entire list is freed.
	Vars is used just as a curur
*/
void	free_env_vars(t_mshell *mshell)
{
	t_env_var	*tmp;
	t_env_var	*head;

	head = mshell->env_vars;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free_env_var(tmp);
	}
}

/**
 @brief Free an env var

 @param mshell A pointer to the env var to free

 @details
	Free the memory allocated for the name and value strings
		only if they are != NULL (just to be sure)
	Free the node itself.
*/
void	free_env_var(t_env_var	*var)
{
	if (var->key)
		free(var->key);
	if (var->value)
		free(var->value);
	free(var);
}
