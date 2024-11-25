/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_matrix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:48:28 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_vars.h"

/**
 @brief Rebuilds the matrix that contains the environment variables

 @param mshell Pointer to main struct

 @details
	Allocated for count + 1 for '/0'
		Memcheck
	Start from the second env var, because the first one is '$0'
	For each envinment var
		Create the stirng [key=value]
		Assign it to the respective position in the new matrix
		(Done with str_concat)

 @return The new matrix
*/
char	**env_to_matrix(t_mshell	*mshell)
{
	char		**result;
	int			i;
	size_t		count;
	t_env_var	*tmp;

	count = count_env_vars(mshell);
	tmp = mshell->env_vars->next;
	i = 0;
	result = ft_calloc(count + 1, sizeof(char *));
	if (!result)
		return (NULL);
	while (tmp)
	{
		result[i] = ft_strconcat(tmp->key, "=", false, false);
		result[i] = ft_strconcat(result[i], tmp->value, true, false);
		i++;
		tmp = tmp->next;
	}
	return (result);
}
