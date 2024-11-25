/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maceccar <maceccar@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 21:09:41 by maceccar          #+#    #+#             */
/*   Updated: 2024/11/25 00:02:22 by maceccar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_VARS_H
# define ENV_VARS_H

# include "minishell.h"

// Forward Declarations
typedef struct s_mshell	t_mshell;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

t_env_var	*new_var(t_mshell *mshell, char *key, char *value, t_bool free_src);

void		free_env_vars(t_mshell *mshell);

t_env_var	*get_var_by_key(t_mshell *mshell, char *target_name);

t_env_var	*update_var(t_env_var *var, char *new_value);

void		free_env_var(t_env_var	*var);

t_bool		delete_env_var(t_mshell	*mshell, t_env_var *to_delete);

size_t		count_env_vars(t_mshell	*mshell);

char		**env_to_matrix(t_mshell	*mshell);

#endif
