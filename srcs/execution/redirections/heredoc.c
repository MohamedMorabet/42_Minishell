/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-mest <oel-mest@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:22:53 by mel-mora          #+#    #+#             */
/*   Updated: 2025/04/11 16:57:20 by oel-mest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/execution.h"

// #define HEREDOC_FILE "/tmp/minihell_heredoc"

// static void	process_heredoc(int fd, char *delimiter, size_t delim_len)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		write(1, "> ", 2);
// 		line = get_next_line(STDIN_FILENO);
// 		if (!line)
// 			break ;
// 		if (ft_strncmp(line, delimiter, delim_len) == 0
// 			&& line[delim_len] == '\n')
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(fd, line, ft_strlen(line));
// 		free(line);
// 	}
// }

// static void	write_heredoc_content(char *delimiter)
// {
// 	int		fd;
// 	size_t	delim_len;

// 	delim_len = ft_strlen(delimiter);
// 	fd = open(HEREDOC_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 	{
// 		perror("HHHH");
// 		exit(1);
// 	}
// 	process_heredoc(fd, delimiter, delim_len);
// 	close(fd);
// }

// static void	redirect_heredoc_input(void)
// {
// 	int	fd;

// 	fd = open(HEREDOC_FILE, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("HHHH");
// 		exit(1);
// 	}
// 	dup2(fd, STDIN_FILENO);
// 	close(fd);
// }

// void	handle_heredoc(char *delimiter)
// {
// 	write_heredoc_content(delimiter);
// 	redirect_heredoc_input();
// }
