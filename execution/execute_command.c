/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:58:19 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/11 15:29:12 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing/minishell.h"

void collect_heredoc(t_cmd *cmd);
int g_status = 0;

void prepare_heredocs(t_ast *node)
{
    if (!node)
        return;
    if (node->type == NODE_COMMAND && node->cmd->heredoc)
        collect_heredoc(node->cmd);
    prepare_heredocs(node->left);
    prepare_heredocs(node->right);
}

void collect_heredoc(t_cmd *cmd) {
    char filename[64];
    static int heredoc_count = 0;

    snprintf(filename, sizeof(filename), "/tmp/minihell_heredoc_%d", heredoc_count++);
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("heredoc file");
        exit(1);
    }

    char buffer[1024];
    while (1) {
        printf("> ");
        if (!fgets(buffer, sizeof(buffer), stdin))
            break;

        if (strncmp(buffer, cmd->heredoc, ft_strlen(cmd->heredoc)) == 0
            && buffer[ft_strlen(cmd->heredoc)] == '\n')
            break;

        write(fd, buffer, strlen(buffer));
    }
    close(fd);

    // Replace the delimiter with the file path (no need to add new field to struct)
    free(cmd->heredoc);  // free old "EOF"
    cmd->heredoc = strdup(filename);  // now holds file path
}

void handle_heredoc_if_needed(t_cmd *cmd) 
{
    if (!cmd->heredoc)
        return;

    if (access(cmd->heredoc, F_OK) == 0) {
        // `heredoc` is a file -> redirect stdin to this file
        int fd = open(cmd->heredoc, O_RDONLY);
        if (fd < 0) {
            perror("open heredoc file");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
}

// envp to array (dont malloc)
char **envp_to_array(EnvNode *envp)
{
	int i = 0;
	EnvNode *tmp = envp;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	char **envp_array = malloc(sizeof(char *) * (i + 1));
	if (!envp_array)
		return (NULL);
	i = 0;
	tmp = envp;
	while (tmp)
	{
		envp_array[i] = ft_strjoin(tmp->var, "=");
		envp_array[i] = ft_strjoin(envp_array[i], tmp->value);
		i++;
		tmp = tmp->next;
	}
	envp_array[i] = NULL;
	return (envp_array);
}

// hundle awk case
void hundle_awk(char **args)
{
	int i = 0;
	while (args[i])
	{
		if (args[i][0] == '\'' || args[i][0] == '"')
		{
			args[i]++;
			args[i][ft_strlen(args[i]) - 1] = '\0';
		}
		i++;
	}
}

int execute_command(t_cmd *cmd, EnvNode **envp)
{
	char **args;
	char *path;
	char **envp_array;

	handle_wildcards(cmd);
	cmd->args = expand_env_vars(cmd->args, *envp);
	args = ft_split(cmd->args, ' ');
	if (!args)
		return (1);
	hundle_awk(args);
	if (args[0])
	{
		if (is_builtin(args[0]))
		{
			int ret = run_builtin(args, envp, cmd);
			g_status = ret;
			free(args);
			return ret;
		}
	}
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	path = NULL;
	if (args)
		path = search_command(args[0], *envp);
	envp_array = envp_to_array(*envp);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		handle_heredoc_if_needed(cmd);
		if (path)
		{
			if (cmd->output)
				redirect_output(cmd->output, cmd->append);
				// redirect_multiple_outputs(cmd->output2, cmd->append);
			if (cmd->input)
				redirect_input(cmd->input);
			if (cmd->output2)
				redirect_multiple_outputs(cmd->output2, cmd->append);
			execve(path, args, envp_array);
		}
		else
		{
			printf("bash: %s: command not found\n", args[0]);
			if (args)
				free(args);
			if (path)
				free(path);
			exit(127);
		}
	}
	waitpid(0, &g_status, 0);
	free(envp_array);
	unlink(cmd->heredoc);
	if (args)
		free(args);
	if (path)
		free(path);
	return (0);
}

int	get_status(void)
{
	return (WEXITSTATUS(g_status));
}