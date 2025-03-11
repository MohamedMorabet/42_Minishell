/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:06:52 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/09 17:19:45 by mel-mora         ###   ########.fr       */
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
        close(fd);
        if (!outputs->next)
            dup2(fd, STDOUT_FILENO);
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