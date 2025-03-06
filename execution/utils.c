/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:28:01 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/06 13:33:10 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

// size_t	ft_strlen(const char *str)
// {
// 	size_t	i;

// 	i = 0;
// 	while (str[i])
// 		i++;
// 	return (i);
// }

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	size_t	i;
// 	size_t	s_len;
// 	char	*sub;

// 	if (!s)
// 		return (NULL);
// 	s_len = ft_strlen(s);
// 	i = 0;
// 	if (start >= s_len)
// 		return (strdup(""));
// 	if (start + len > s_len)
// 		len = s_len - start;
// 	sub = (char *)malloc(len + 1);
// 	if (!sub)
// 		return (NULL);
// 	else
// 	{
// 		while (i < len)
// 		{
// 			sub[i] = s[start + i];
// 			i++;
// 		}
// 	}
// 	sub[i] = '\0';
// 	return (sub);
// }

// static size_t	count_words(char const *s, char c)
// {
// 	int		i;
// 	size_t	count;

// 	i = 0;
// 	count = 0;
// 	if (!s || !s[0])
// 		return (0);
// 	if (i == 0 && s[0] != c)
// 	{
// 		count++;
// 	}
// 	i++;
// 	while (s[i] != '\0')
// 	{
// 		if (s[i] != c && s[i - 1] == c)
// 			count++;
// 		i++;
// 	}
// 	return (count);
// }

// static char	**free_table(char **tab, size_t j)
// {
// 	size_t	i;

// 	i = 0;
// 	while (j > 0)
// 		free(tab[--j]);
// 	free(tab);
// 	return (NULL);
// }

// static char	**fill_table(char **tab, const char *s, char c, size_t count)
// {
// 	size_t	i;
// 	size_t	a;
// 	size_t	j;
// 	char	*word;

// 	i = 0;
// 	a = 0;
// 	j = 0;
// 	while (count > 0)
// 	{
// 		while (s[i] == c)
// 			i++;
// 		a = i;
// 		while (s[i] && s[i] != c)
// 			i++;
// 		word = ft_substr(s, a, i - a);
// 		if (!word)
// 			return (free_table (tab, j));
// 		tab[j++] = word;
// 		count--;
// 	}
// 	tab[j] = NULL;
// 	return (tab);
// }

// char	**ft_split(char const *s, char c)
// {
// 	char		**tab;
// 	size_t		count;

// 	if (!s)
// 		return (NULL);
// 	count = count_words(s, c);
// 	tab = (char **)malloc((count + 1) * sizeof(char *));
// 	if (!tab)
// 		return (NULL);
// 	return (fill_table(tab, s, c, count));
// }