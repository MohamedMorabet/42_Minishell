/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:34:16 by mel-mora          #+#    #+#             */
/*   Updated: 2025/04/10 20:38:45 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/execution.h"

void	minishell(t_ast *root, t_envnode **envp)
{
	prepare_heredocs(root, envp);
	execute_ast(root, envp);
}

int	execute_ast(t_ast *node, t_envnode **envp)
{
	if (!node)
		return (1);
	if (node->type == NODE_COMMAND)
		return (exec_command_node(node, envp));
	if (node->type == NODE_PIPE)
		return (exec_pipe_node(node, envp));
	if (node->type == NODE_AND)
		return (exec_and_node(node, envp));
	if (node->type == NODE_OR)
		return (exec_or_node(node, envp));
	if (node->type == NODE_SUB)
		return (exec_sub_node(node, envp));
	return (0);
}
