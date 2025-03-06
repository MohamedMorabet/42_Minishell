/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:58:19 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/06 15:20:30 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing/minishell.h"

void redirect_output(char *file, int append)
{
    int flags;
	
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
    int fd = open(file, flags, 0644);
    if (fd < 0)
	{
        perror("open output file");
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void redirect_input(char *file)
{
    int fd;
	
	fd = open(file, O_RDONLY);
    if (fd < 0)
	{
        perror("open input file");
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void redirect_multiple_outputs(t_output *outputs, int append)
{
	int	fd;
	int	flags;

    flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
    while (outputs) 
	{
        fd = open(outputs->file, flags, 0644);
        if (fd < 0)
		{
            perror("open output file");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO); // Warning: only last one matters for stdout
        close(fd);
        outputs = outputs->next;
    }
}

void handle_heredoc(char *delimiter)
{
    char buffer[1024];
    int fd;
	
	fd = open("/tmp/minihell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
	{
        perror("heredoc temp file");
        exit(1);
    }
    while (1)
	{
        printf("> ");
        if (!fgets(buffer, sizeof(buffer), stdin))
            break;
        if (strncmp(buffer, delimiter, strlen(delimiter)) == 0 && buffer[strlen(delimiter)] == '\n')
            break;
        write(fd, buffer, strlen(buffer));
    }
    close(fd);

    // Reopen as input
    fd = open("/tmp/minihell_heredoc", O_RDONLY);
    if (fd < 0)
	{
        perror("reopen heredoc file");
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}


int execute_command(t_cmd *cmd, char **envp)
{
	char **args;
	char *path;

	args = ft_split(cmd->args, ' ');
	if (!args)
		return (1);
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	path = search_command(args[0], envp);
	if (pid == 0)
	{
		if (path)
		{
			if (cmd->output)
				redirect_output(cmd->output, cmd->append);
			if (cmd->input)
				redirect_input(cmd->input);
			if (cmd->output2)
				redirect_multiple_outputs(cmd->output2, cmd->append);
			if (cmd->heredoc)
				handle_heredoc(cmd->heredoc);
			execve(path, args, envp);
		}
		else
			printf("Command not found: %s\n", args[0]);\
	}
	wait(NULL);
	free(path);
	free(args);
	return (1);
}
