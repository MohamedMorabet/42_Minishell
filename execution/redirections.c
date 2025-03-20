/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:06:52 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/20 17:17:11 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_v2/minishell.h"

#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

void redirect_output(t_output *output_list, int append)
{
    int fd = -1;
    t_output *current = output_list;

    while (current)
    {
        int flags = O_WRONLY | O_CREAT | O_CLOEXEC;
        if (append)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;

        // Close previous file descriptor (except the last one)
        if (fd != -1)
            close(fd);

        fd = open(current->file, flags, 0644);
        if (fd < 0)
        {
            perror("open output file");
            exit(1);
        }
        current = current->next;
    }

    if (fd != -1)
    {
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2 output file");
            close(fd);
            exit(1);
        }
        close(fd);
    }
}

void redirect_input(t_output *input_list)
{
    int fd = -1;
    t_output *current = input_list;

    while (current)
    {
        // Close previous file descriptor (except the last one)
        if (fd != -1)
            close(fd);

        fd = open(current->file, O_RDONLY | O_CLOEXEC);
        if (fd < 0)
        {
            perror("open input file");
            exit(1);
        }
        current = current->next;
    }

    if (fd != -1)
    {
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2 input file");
            close(fd);
            exit(1);
        }
        close(fd);
    }
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

void redirect_input1(char *file)
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