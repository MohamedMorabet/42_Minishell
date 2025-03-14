/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:14:41 by mel-mora          #+#    #+#             */
/*   Updated: 2024/10/30 15:31:26 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*lastel;

	lastel = lst;
	while (lastel && lastel->next != NULL)
	{
		lastel = lastel->next;
	}
	return (lastel);
}
