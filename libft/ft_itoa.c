/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 22:29:05 by sel-jari          #+#    #+#             */
/*   Updated: 2024/11/01 22:29:07 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	digit_nbr(int n)
{
	int	count;

	count = 1;
	while (n / 10 != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static void	to_char(char *ptr, long l, int number_of_chars)
{
	ptr[number_of_chars] = 0;
	if (!l)
		ptr[0] = '0';
	while (l)
	{
		ptr[number_of_chars - 1] = l % 10 + 48;
		l = l / 10;
		number_of_chars--;
	}
}

char	*ft_itoa(int n)
{
	char	*ptr;
	int		number_of_chars;
	long	l;

	l = n;
	number_of_chars = digit_nbr(l);
	if (l < 0)
	{
		number_of_chars++;
		ptr = malloc(number_of_chars + 1);
		if (!ptr)
			return (0);
		ptr[0] = '-';
		l *= -1;
	}
	else
		ptr = malloc(number_of_chars + 1);
	if (!ptr)
		return (0);
	to_char(ptr, l, number_of_chars);
	return (ptr);
}
