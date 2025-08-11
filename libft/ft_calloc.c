/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 22:26:58 by sel-jari          #+#    #+#             */
/*   Updated: 2024/11/01 22:27:04 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t ne, size_t size)
{
	void	*ptr;

	ptr = malloc(ne * size);
	if (ptr)
		ft_bzero(ptr, size * ne);
	return (ptr);
}
