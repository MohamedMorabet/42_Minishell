/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:02:37 by mel-mora          #+#    #+#             */
/*   Updated: 2025/04/05 16:18:04 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"
#include "../../../../includes/execution.h"

// env (print all environment variables)
int	builtin_env(t_envnode *envp)
{
	while (envp)
	{
		if (envp->value && (ft_strcmp(envp->value, "") != 0))
			printf("%s=%s\n", envp->var, envp->value);
		envp = envp->next;
	}
	return (0);
}
