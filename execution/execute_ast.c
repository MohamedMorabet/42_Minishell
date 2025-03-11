/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:34:16 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/11 15:31:04 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing/minishell.h"

// execute pipe function 
int execute_pipe(t_ast *left, t_ast *right, EnvNode **envp)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execute_ast(left, envp);
        exit(0);
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        return 1;
    }

    if (pid2 == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execute_ast(right, envp);
        exit(0);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    return 0;
}

void    minishell(t_ast *root, EnvNode **envp)
{
    prepare_heredocs(root);
    execute_ast(root, envp);
}

int execute_ast(t_ast *node, EnvNode **envp)
{
    if (!node)
        return 0;

    if (node->type == NODE_COMMAND)
        return (execute_command(node->cmd, envp));
    if (node->type == NODE_PIPE)
        return execute_pipe(node->left, node->right, envp);

    if (node->type == NODE_AND)
    {
        if (execute_ast(node->left, envp) == 0)
            return execute_ast(node->right, envp);
        return 1;
    }

    if (node->type == NODE_OR)
    {
        if (execute_ast(node->left, envp) == 1)
            return execute_ast(node->right, envp);
        return 0;
    }
    return 0;
}
