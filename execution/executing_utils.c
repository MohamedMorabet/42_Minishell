/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:35:42 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/06 14:00:38 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing/minishell.h"

char	*join_path(const char *dir, const char *cmd)
{
	size_t	len = ft_strlen(dir) + 1 + ft_strlen(cmd);
	char	*full = malloc(len + 1);
	if (!full)
		return (NULL);
	full[0] = '\0';
	strcat(full, dir);
	strcat(full, "/");
	strcat(full, cmd);
	return (full);
}

// Manual PATH splitting + command search
char	*search_command(char *cmd, char **envp)
{
	// Case 1: Command is already absolute/relative path (contains '/')
	if (strchr(cmd, '/') != NULL)
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}

	// Case 2: Search PATH
	char	*path = getenv("PATH");
	if (!path)
		return (NULL);

	// Manually split PATH (cannot use strtok)
	char	*start = path;
	while (*start)
	{
		char *colon = strchr(start, ':');
		if (!colon)
			colon = start + ft_strlen(start); // Last segment

		// Build directory string (current PATH entry)
		char dir[1024];
		size_t len = colon - start;
		if (len >= sizeof(dir))
			len = sizeof(dir) - 1;
		strncpy(dir, start, len);
		dir[len] = '\0';

		// Join dir + "/" + cmd
		char *full_path = join_path(dir, cmd);
		if (!full_path)
			return (NULL);

		if (access(full_path, X_OK) == 0)
			return (full_path); // Command found!

		free(full_path);
		if (*colon == '\0')
			break; // End of PATH
		start = colon + 1;
	}
	return (NULL); // Command not found
}