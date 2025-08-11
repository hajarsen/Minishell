/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 22:42:59 by sel-jari          #+#    #+#             */
/*   Updated: 2025/06/16 15:28:38 by sel-jari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *str, char c)
{
	int	i;
	int	words;

	words = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			i++;
			continue ;
		}
		words++;
		while (!(str[i] == c) && str[i])
			i++;
	}
	return (words);
}

static void	ft_strncpy(char *dest, const char *src, int n)
{
	int	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

static int	find_and_count(char const *str, char c, int *i)
{
	int	chars;

	if (str[*i] == c)
	{
		(*i)++;
		return (-1);
	}
	chars = 0;
	while (!(str[*i] == c) && str[*i])
	{
		chars++;
		(*i)++;
	}
	return (chars);
}

static int	malloc_fill(char **arr, char const *str, char c)
{
	int	i;
	int	chars;
	int	k;

	i = 0;
	k = 0;
	while (str[i])
	{
		chars = find_and_count(str, c, &i);
		if (chars == -1)
			continue ;
		arr[k] = malloc(chars + 1);
		if (arr[k] == 0)
		{
			i = 0;
			while (i < k)
				free(arr[i++]);
			free(arr);
			return (0);
		}
		ft_strncpy(arr[k], str + i - chars, chars);
		k++;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;

	if (!s)
		return (NULL);
	arr = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	if (!malloc_fill(arr, s, c))
		return (NULL);
	arr[count_words(s, c)] = 0;
	return (arr);
}
/*
#include <stdio.h>
int	main()
{
	char const a[] = "saad eljarida,,,asd asd";
	char **arr;
	int	i;
	int	words;
	char k= ',';
	i = 0;
	arr = ft_split(a,k);
	words = count_words(a,k);
	while(i < words)
	{
		printf("%s\n",arr[i]);	
		free(arr[i]);
		i++;
	}
	free(arr);
}
*/
