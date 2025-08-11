/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 22:43:30 by sel-jari          #+#    #+#             */
/*   Updated: 2024/11/01 22:43:36 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*d;
	size_t	sz;

	sz = ft_strlen(s) + 1;
	d = (char *)malloc(sz);
	if (d)
		ft_memcpy(d, s, sz);
	return (d);
}
