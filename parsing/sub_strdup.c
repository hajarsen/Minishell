/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_strdup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 07:47:48 by sel-jari          #+#    #+#             */
/*   Updated: 2025/08/19 07:47:50 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr_(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_calloc(1, sizeof(char)));
	if (ft_strlen(s) <= len - start)
		ptr = malloc(ft_strlen(s) - start + 1);
	else
		ptr = malloc(len + 1);
	if (!ptr)
		return (0);
	while (i < len && s[start + i])
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_strdup_(const char *s)
{
	char	*d;
	size_t	sz;

	sz = ft_strlen(s) + 1;
	d = (char *)malloc(sz);
	if (d)
		ft_memcpy(d, s, sz);
	return (d);
}
