/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 04:56:34 by hsennane          #+#    #+#             */
/*   Updated: 2025/08/12 04:56:35 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	overflow_check(long long num, int sign, int digit)
{
	long long	max_divider;
	long long	min_divider;

	max_divider = LLONG_MAX / 10;
	min_divider = LLONG_MIN / 10;
	if (sign == 1)
	{
		if (num > max_divider || (num == max_divider && digit > LLONG_MAX % 10))
			return (1);
	}
	else
	{
		if (num < min_divider || (num == min_divider && digit > -(LLONG_MIN
					% 10)))
			return (1);
	}
	return (0);
}

static int	skip_whitespace_and_sign(const char *str, int *index, int *sign)
{
	while (str[*index] && ((str[*index] >= 9 && str[*index] <= 13)
			|| str[*index] == 32))
		(*index)++;
	if (str[*index] && (str[*index] == '+' || str[*index] == '-'))
	{
		if (str[*index] == '-')
			*sign = -1;
		(*index)++;
	}
	return (0);
}

static long long	accumulate_number(const char *str, int *index, int sign,
		int *overflow)
{
	long long	num;

	num = 0;
	while (str[*index] && (str[*index] >= '0' && str[*index] <= '9'))
	{
		if (overflow_check(num, sign, str[*index] - '0'))
		{
			*overflow = 1;
			return (0);
		}
		if (sign == 1)
			num = (num * 10) + (str[*index] - '0');
		else
			num = (num * 10) - (str[*index] - '0');
		(*index)++;
	}
	*overflow = 0;
	return (num);
}

long long	my_ft_atoll(const char *str, int *overflow)
{
	int			i;
	int			sign;
	long long	num;

	i = 0;
	sign = 1;
	skip_whitespace_and_sign(str, &i, &sign);
	num = accumulate_number(str, &i, sign, overflow);
	return (num);
}
