/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:34:21 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/11 15:38:00 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing/minishell.h"

int is_builtin(char *cmd) 
{
    return (
        !strcmp(cmd, "echo") ||
        !strcmp(cmd, "cd") ||
        !strcmp(cmd, "pwd") ||
        !strcmp(cmd, "export") ||
        !strcmp(cmd, "unset") ||
        !strcmp(cmd, "env") ||
        !strcmp(cmd, "exit")
    );
}

int run_builtin(char **args, EnvNode **envp, t_cmd *cmd)
{
	if (!strcmp(args[0], "echo"))
		return builtin_echo(cmd->args);
	else if (!strcmp(args[0], "cd"))
		return builtin_cd(args);
	else if (!strcmp(args[0], "pwd"))
		return builtin_pwd();
	else if (!strcmp(args[0], "export"))
		return builtin_export(args, envp);  // Change to **envp
	else if (!strcmp(args[0], "unset"))
		return builtin_unset(args, envp);   // Change to **envp
	else if (!strcmp(args[0], "env"))
		return builtin_env(*envp);  // Pass single pointer (no modification needed)
	else if (!strcmp(args[0], "exit"))
		return builtin_exit();
	return (1);
}


// echo with -n option
int builtin_echo(char *args)
{
    int newline;
    int i;
    
    i = 5;
    newline = 1;
    if (args[i] == '-' && args[i + 1] == 'n')
    {
        newline = 0;
        i+=3;
    }
    while (args[i])
    {
        if (args[i] != '"' && args[i] != '\'')
            printf("%c", args[i]);
        i++;
    }
    if (newline)
        printf("\n");
    return 0;
}

// cd with relative/absolute path
int builtin_cd(char **args)
{
    if (!args[1])
    {
        fprintf(stderr, "minihell: cd: missing argument\n");
        return 1;
    }
    if (chdir(args[1]) != 0)
    {
        printf("bash: cd: %s: No such file or directory\n", args[1]);
        return 1;
    }
    return 0;
}

// pwd (print working directory)
int builtin_pwd()
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    else
        perror("minihell: pwd");
    return 0;
}

// export env in our node list (dosnt use strtok) with msg of error
EnvNode *find_env_var(EnvNode *envp, const char *var)
{
    while (envp)
    {
        if (strcmp(envp->var, var) == 0)
            return envp; // Found existing variable
        envp = envp->next;
    }
    return NULL; // Not found
}

int is_valid_var_name(const char *var)
{
    if (!var || (!isalpha(var[0]) && var[0] != '_')) // Must start with a letter or '_'
        return 0;

    for (int i = 1; var[i]; i++) // Remaining chars can be letters, numbers, or '_'
    {
        if (!isalnum(var[i]) && var[i] != '_')
            return 0;
    }
    return 1;
}

int builtin_export(char **args, EnvNode **envp)
{
    int i;
    char *eq;
    EnvNode *node;

    if (!envp) // If envp itself is NULL (should not happen in normal cases)
        return 1;

    i = 0;
    while (args[++i])
    {
        eq = strchr(args[i], '=');
        if (eq == NULL)
        {
            printf("minishell: export: `%s': not a valid identifier\n", args[i]);
            return 1;
        }

        size_t var_len = eq - args[i];
        char *var_name = strndup(args[i], var_len);
        if (!var_name)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        // Validate variable name
        if (!is_valid_var_name(var_name))
        {
            printf("minishell: export: `%s': not a valid identifier\n", var_name);
            free(var_name);
            return 1;
        }

        // If envp is NULL (first variable to be exported), create a new list
        if (*envp == NULL)
        {
            *envp = create_node(args[i]); // Create first node
            free(var_name);
            continue;
        }

        // Search if the variable already exists
        node = find_env_var(*envp, var_name);
        if (node)
        {
            free(node->value);
            node->value = strdup(eq + 1);
        }
        else
        {
            node = create_node(args[i]); // Create a new node
            add_node(envp, node); // Add it to the list
        }

        free(var_name);
    }
    return 0;
}


// unset (remove env variable) with msg of error
int builtin_unset(char **args, EnvNode **envp)
{
    int i;
    EnvNode *prev;
    EnvNode *tmp;

    if (!envp || !*envp) // Ensure envp is not NULL
        return 1;

    i = 0;
    while (args[++i])
    {
        prev = NULL;
        tmp = *envp; // Start from the head of the list

        while (tmp)
        {
            if (!strcmp(tmp->var, args[i]))
            {
                if (prev)
                    prev->next = tmp->next; // Bypass the node
                else
                    *envp = tmp->next; // Update head if first node is removed

                free(tmp->var);
                free(tmp->value);
                free(tmp);
                break;
            }
            prev = tmp;
            tmp = tmp->next;
        }
        if (!tmp)
            fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n", args[i]);
    }
    return 0;
}


// env (print all environment variables)
int builtin_env(EnvNode *envp)
{
    while (envp)
    {
        printf("%s=%s\n", envp->var, envp->value);
        envp = envp->next;
    }
    return 0;
}
// exit (exit shell)
int builtin_exit()
{
    // colored professionnel msg for exiting minishell
    printf("\033[1;31mGoodbye! 🚀\033[0m\n");
    exit(0);
}