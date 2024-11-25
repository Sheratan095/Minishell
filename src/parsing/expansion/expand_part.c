/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_part.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:20:39 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define DOLLAR 36
#define DOUBLE_QUOTE 34
#define SINGLE_QUOTE 39

static char	*replace_var(t_mshell *mshell, char *arg, int start, int end);
static char	*rewrite_arg(char *new_arg, char *old_arg, t_env_var *var, int i);

/**
 @brief Expand just a part o the current argument

 @param mshell Pointer to main structure
 @param arg_part Pointer to part of argument to expand

 @details
	Pass through the part
		if ther's a dollar => new var key
			skip the dollar
			if ther's dollar
				the var name is just ? (in normal bash, if ther's a dollar
				is automatically interpreted as the defualt var $?)
				try to replace var
			else
				reach the end of var key (space, another dollar or end of string)
				try to replace var

 @note The substring is freed and overwritten in replace_var()
		This function isn't static because it's included by heredoc.c
 
 @retval Command
 @retval NULL if something went wrong with allocation
			or there're some unclosed quotes
*/
char	*expand_single_part(t_mshell *mshell, char	*part)
{
	int		start_of_var_name;
	int		i;

	i = 0;
	while (part[i])
	{
		if (part[i] == DOLLAR)
		{
			i++;
			start_of_var_name = i;
			if (part[i] == '?')
				part = replace_var(mshell, part, start_of_var_name, i);
			else
			{
				while (part[i] && (ft_isalnum(part[i]) || part[i] == '_'))
					i++;
				part = replace_var(mshell, part, start_of_var_name, i - 1);
			}
			i = 0;
		}
		else
			i++;
	}
	return (part);
}

/**
 @brief Rewrite the arg with expandend var in the new string

 @param mshell Pointer to mshell
 @param arg Pointer to source string
 @param start Starting index of var name
 @param end Ending index of var name

 @details
	Retrieve the name of the var with substring
	Check malloc in substring
	Free the substring
	Retrieve the var from its name
	Calculate the length of new arg
		length of source - name of var(contains also $)
	If the var exist
		Add to the final length the length of the corresponding value
	Else
		Subtract 1 to avoid $
	Allocate resulting string and memcheck
	Write everything back to the new string
	Free the original string 

 @retval source if something wrong
 @retval Pointer to new arg
*/
static char	*replace_var(t_mshell *mshell, char *arg, int start, int end)
{
	char		*resulting_arg;
	int			resulting_len;
	char		*target_var;
	t_env_var	*var;

	target_var = ft_substr(arg, start, end - start + 1);
	if (!target_var)
		return (arg);
	var = get_var_by_key(mshell, target_var);
	free(target_var);
	resulting_len = ft_strlen(arg) - (end - start);
	if (var)
		resulting_len += ft_strlen(var->value);
	else
		resulting_len --;
	resulting_arg = ft_calloc(resulting_len + 1, sizeof(char));
	if (!resulting_arg)
		return (arg);
	resulting_arg = rewrite_arg(resulting_arg, arg, var, 0);
	free(arg);
	return (resulting_arg);
}

/**
 @brief Rewrite the arg with expandend var in the new string

 @param new_arg Pointer to string where to copy
 @param old_arg Pointer to string from which to copy
 @param var Pointer to var that contains the value
 @param i (in parameters just form norminette)

 @details
	i is used to travese the old argument
	j is used to travese the new argument
	k is used to travese the value of the var
	
	Copy old values to new arg until reach the DOLLAR
	Skip the DOLLAR
	if there isn't '?')
		Skip the var name in the old string to update j
	else
		skip just it (in normal bash, if ther's a dollar
			is automatically interpreted as the defualt var $?)
	If var exist
		write it in the new string
	Else
		don't do anithing, it will be just deleted from the string
	write all the rest of the string

 @retval Pointer to new arg
*/
static char	*rewrite_arg(char *new_arg, char *old_arg, t_env_var *var, int i)
{
	int	j;
	int	k;
	int	value_len;

	j = 0;
	k = 0;
	while (old_arg[i] && old_arg[i] != DOLLAR)
		new_arg[j++] = old_arg[i++];
	i++;
	if (old_arg[i] != '?')
	{
		while (old_arg[i] && (ft_isalnum(old_arg[i]) || old_arg[i] == '_'))
			i++;
	}
	else
		i++;
	if (var)
	{
		value_len = ft_strlen(var->value);
		while (k < value_len)
			new_arg[j++] = var->value[k++];
	}
	while (old_arg[i])
		new_arg[j++] = old_arg[i++];
	return (new_arg);
}
