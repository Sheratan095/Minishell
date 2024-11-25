/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:14:42 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 @brief Update the var with its new value

 @param var Pointer to var to modify
 @param new_value Pointer to the new value

 @details
	Free the previus value of the var
	Assing the new value to var
	Memcheck for strdup()
	
 @retval The modified variable
 @retval NULL if strdup() fail to allocate 
*/
t_env_var	*update_var(t_env_var *var, char *new_value)
{
	free(var->value);
	if (new_value)
		var->value = ft_strdup(new_value);
	else
		var->value = ft_strdup("");
	if (!var->value)
		return (NULL);
	return (var);
}
