/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_to_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:46:22 by oel-mest          #+#    #+#             */
/*   Updated: 2025/03/11 02:16:35 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast *parse_command(t_token **tokens)
{
    t_ast *node = create_ast_node(NODE_COMMAND);
    node->cmd = create_cmd_node();
    while (*tokens)
    {
        if ((*tokens)->type == TOKEN_WORD || (*tokens)->type == SINGLE_Q || (*tokens)->type == DOUBLE_Q)
        {
            add_argument(node->cmd, *tokens);
            *tokens = (*tokens)->next;
        }
        else if ((*tokens)->type == TOKEN_REDIRECT_IN)
        {
            *tokens = (*tokens)->next;
            if (*tokens && (*tokens)->type == TOKEN_WORD) {
                free(node->cmd->input);
                node->cmd->input = strdup((*tokens)->value);
                *tokens = (*tokens)->next;
            }
        } else if ((*tokens)->type == TOKEN_REDIRECT_OUT || (*tokens)->type == TOKEN_APPEND) {
            int is_append = ((*tokens)->type == TOKEN_APPEND);
            *tokens = (*tokens)->next;
            if (*tokens && (*tokens)->type == TOKEN_WORD) {
                free(node->cmd->output);
                node->cmd->output = strdup((*tokens)->value);
                add_output(&node->cmd->output2, create_output_node((*tokens)->value));
                node->cmd->append = is_append;
                *tokens = (*tokens)->next;
            }
        }
        else if ((*tokens)->type == TOKEN_HEREDOC)
        {
            *tokens = (*tokens)->next;
            if (*tokens && (*tokens)->type == TOKEN_WORD) {
                free(node->cmd->heredoc);
                node->cmd->heredoc = strdup((*tokens)->value);
                *tokens = (*tokens)->next;
            }
        }
        else
        {
            break;
        }
    }

    return node;
}

t_ast *parse_pipeline(t_token **tokens)
{
    t_ast *left = parse_command(tokens);

    while (*tokens && (*tokens)->type == TOKEN_PIPE) {
        t_ast *node = create_ast_node(NODE_PIPE);
        *tokens = (*tokens)->next;
        t_ast *right = parse_command(tokens);
        node->left = left;
        node->right = right;
        left = node;
    }

    return left;
}

t_ast *parse_logical(t_token **tokens)
{
    t_ast *left = parse_pipeline(tokens);

    while (*tokens && ((*tokens)->type == TOKEN_AND || (*tokens)->type == TOKEN_OR)) {
        t_ast *node = create_ast_node(
            (*tokens)->type == TOKEN_AND ? NODE_AND : NODE_OR
        );
        *tokens = (*tokens)->next;
        t_ast *right = parse_pipeline(tokens);
        node->left = left;
        node->right = right;
        left = node;
    }

    return left;
}

t_ast *parse(t_token *tokens)
{
    return parse_logical(&tokens);
}