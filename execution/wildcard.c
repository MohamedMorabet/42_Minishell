/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:18:40 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/11 00:59:52 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing/minishell.h"

// Read output of `ls`
char *get_ls_output()
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return NULL;

    pid_t pid = fork();
    if (pid == -1)
        return NULL;

    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execlp("ls", "ls", NULL);
        exit(1);
    }

    // Parent process
    close(pipefd[1]);
    char *buffer = malloc(4096);
    if (!buffer)
        return NULL;

    read(pipefd[0], buffer, 4096);
    close(pipefd[0]);
    wait(NULL);
    return buffer;
}

// Check if filename matches wildcard pattern (* at start or end)
int matches_pattern(const char *filename, const char *pattern)
{
    int file_len = strlen(filename);
    int pat_len = strlen(pattern);

    if (pattern[0] == '*' && pat_len > 1) // *X (MUST end with X)
        return (file_len >= pat_len - 1) &&
               (strcmp(filename + file_len - (pat_len - 1), pattern + 1) == 0);
    if (pattern[pat_len - 1] == '*') // X* (MUST start with X)
        return strncmp(filename, pattern, pat_len - 1) == 0;
    if (pattern[0] == '*' && pattern[pat_len - 1] == '*') // *X* (Contains X)
        return strstr(filename, pattern + 1) != NULL;
    return strcmp(filename, pattern) == 0; // Exact match
}

// Extract filenames matching a wildcard pattern (NO strtok)
char *filter_ls_output(const char *ls_output, const char *pattern)
{
    char *filtered = malloc(1024);
    if (!filtered)
        return NULL;

    filtered[0] = '\0';

    char *ls_copy = strdup(ls_output);
    if (!ls_copy)
        return NULL;

    char *start = ls_copy;
    char *end;
    while (*start)
    {
        // Find end of the current filename
        end = strchr(start, '\n');
        if (!end)
            end = start + strlen(start);

        // Copy the filename
        char filename[256];
        int len = end - start;
        if (len >= 256) len = 255; // Avoid overflow
        strncpy(filename, start, len);
        filename[len] = '\0';

        // Match pattern
        if (matches_pattern(filename, pattern))
        {
            strcat(filtered, filename);
            strcat(filtered, " ");
        }

        // Move to next filename
        if (*end == '\n')
            start = end + 1;
        else
            break;
    }

    free(ls_copy);

    if (strlen(filtered) > 0)
        filtered[strlen(filtered) - 1] = '\0'; // Remove last space

    return filtered;
}

// Handles wildcard expansion in command arguments
void handle_wildcards(t_cmd *cmd)
{
    int i;
    char **args = ft_split(cmd->args, ' ');
    if (!args)
        return;

    char new_args[1024] = "";
    char *ls_output = get_ls_output();
    if (!ls_output)
        return;

    i = 0;
    while (args[i])
    {
        // printf("arg%d: %s\n", i+1, args[i]);

        if (strchr(args[i], '*')) // If wildcard is found
        {
            char *expanded = filter_ls_output(ls_output, args[i]); // Expand only this arg
            // printf("expanded: %s\n", expanded);
            if (expanded && expanded[0] != '\0') // Check if anything matched
            {
                strcat(new_args, expanded);
                strcat(new_args, " ");
                free(expanded);
            }
            else
            {
                strcat(new_args, args[i]);
                strcat(new_args, " ");
            }
        }
        else
        {
            strcat(new_args, args[i]);
            strcat(new_args, " ");
        }
        i++;
    }

    new_args[strlen(new_args) - 1] = '\0';
    free(cmd->args);
    cmd->args = strdup(new_args); // Replace cmd->args

    free(ls_output);
}
