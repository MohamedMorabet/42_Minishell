/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-mest <oel-mest@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:52:34 by oel-mest          #+#    #+#             */
/*   Updated: 2025/02/19 15:25:37 by oel-mest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_indentation(int depth, int is_last)
{
    for (int i = 0; i < depth; i++) {
        if (i == depth - 1) {
            printf(is_last ? "└── " : "├── ");
        } else {
            printf("    ");
        }
    }
}

void print_cmd(t_cmd *cmd, int depth, int is_last)
{
    print_indentation(depth, is_last);
    printf("COMMAND:\n");

    // Print arguments
    print_indentation(depth + 1, 0);
    printf("ARGS: %s\n", cmd->args);

    // Print input redirection
    if (cmd->input) {
        print_indentation(depth + 1, 0);
        printf("INPUT: %s\n", cmd->input);
    }

    // Print output redirection
    if (cmd->output) {
        print_indentation(depth + 1, 0);
        printf("OUTPUT: %s (%s)\n", cmd->output, cmd->append ? "APPEND" : "TRUNCATE");
        print_indentation(depth + 1, 0);
        print_outputs(cmd->output2);
    }

    // Print heredoc
    if (cmd->heredoc) {
        print_indentation(depth + 1, 0);
        printf("HEREDOC: %s\n", cmd->heredoc);
    }
}

void print_ast(t_ast *node, int depth, int is_last)
{
    if (!node) return;

    // Print the current node
    print_indentation(depth, is_last);
    switch (node->type) {
        case NODE_COMMAND:
            printf("COMMAND NODE:\n");
            print_cmd(node->cmd, depth + 1, 1);
            break;
        case NODE_PIPE:
            printf("PIPE NODE:\n");
            break;
        case NODE_AND:
            printf("AND NODE (&&):\n");
            break;
        case NODE_OR:
            printf("OR NODE (||):\n");
            break;
        default:
            printf("UNKNOWN NODE:\n");
            break;
    }

    // Recursively print children
    if (node->left) {
        print_ast(node->left, depth + 1, node->right == NULL);
    }
    if (node->right) {
        print_ast(node->right, depth + 1, 1);
    }
}

void print_ast_tree(t_ast *root)
{
    printf("AST TREE:\n");
    print_ast(root, 0, 1);
}