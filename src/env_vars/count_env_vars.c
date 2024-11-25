/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_env_vars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:50:32 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_vars.h"

/**
 @brief Count the environment variables

 @param mshell Pointer to main struct

 @details
	count-- at last is done to avoid considering also $0

 @return Total number of env var
*/
size_t	count_env_vars(t_mshell	*mshell)
{
	size_t		count;
	t_env_var	*tmp;

	count = 0;
	tmp = mshell->env_vars;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	count--;
	return (count);
}
