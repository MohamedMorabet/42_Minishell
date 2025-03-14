/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:50:34 by mel-mora          #+#    #+#             */
/*   Updated: 2024/10/31 09:44:52 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*return_val;

	return_val = malloc(sizeof(t_list));
	if (!return_val)
		return (NULL);
	return_val->content = content;
	return_val->next = NULL;
	return (return_val);
}
