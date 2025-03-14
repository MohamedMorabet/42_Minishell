/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 07:46:34 by mel-mora          #+#    #+#             */
/*   Updated: 2024/10/31 11:20:49 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (!lst || !del)
		return ;
	while (*lst && lst)
	{
		tmp = (*lst)->next;
		ft_lstdelone (*lst, del);
		*lst = tmp;
	}
	*lst = NULL;
}
