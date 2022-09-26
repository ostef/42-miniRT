/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 17:24:06 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/26 17:24:31 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	ft_pow(int nb, int power)
{
	int	i;
	int	result;

	i = 0;
	result = 1;
	while (i < power)
	{
		result = result * nb;
		i++;
	}
	return (result);
}

static t_bool	ft_is_decimal_number(t_cstr str)
{
	t_uint	i;

	i = 0;
	if (str[0] == '-')
		i++;
	while (ft_is_digit(str[i]))
		i++;
	if (str[i] == '.')
		i++;
	while (ft_is_digit(str[i]))
		i++;
	return (str[i] == '\0');
}

t_f32	ft_atof(t_cstr str)
{
	t_str	str_int;
	t_f32	result;
	t_s64	integer;
	t_uint	index_decimal;
	t_uint	i;

	if (!ft_is_decimal_number(str))
		return (NAN);
	index_decimal = 0;
	i = ft_strlen(str) - 1;
	while (str[i] && str[i] != '.' && i > 0)
	{
		index_decimal++;
		i--;
	}
	if (i == 0)
		index_decimal = 0;
	str_int = ft_strdup(str, ft_heap());
	if (index_decimal != 0)
		ft_memmove(str_int + i, str_int + i + 1, ft_strlen(str_int) - i);
	ft_str_to_s64(str_int, &integer);
	ft_free(str_int, ft_heap());
	result = (t_f32)integer / ft_pow(10, index_decimal);
	return (result);
}
