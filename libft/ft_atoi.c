/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:34:30 by mel-mora          #+#    #+#             */
/*   Updated: 2024/11/01 17:34:27 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_overflow(unsigned long result, int nb, int sign)
{
	if (sign == 1)
	{
		if ((result >= 922337203685477580 && nb > 7) 
			|| result >= 922337203685477581)
			return (-1);
	}
	if (sign == -1)
	{
		if ((result >= 922337203685477580 && nb > 8) 
			|| result > 922337203685477581)
			return (0);
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int				i;
	int				sign;
	unsigned long	result;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (check_overflow(result, str[i] - '0', sign) != 1)
			return (check_overflow(result, str[i] - '0', sign));
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
