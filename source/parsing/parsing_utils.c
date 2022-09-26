/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:06:29 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/23 15:21:50 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_str	ft_dup_split_str(t_pcstr split_str)
{
	return ft_strndup(split_str.data, split_str.len, ft_heap());
}

void	ft_error(t_error code, t_uint nb_line)
{
	ft_fprintln (STDERR, "Error");
	if (code == ERR_NO_FILENAME)
		ft_fprintln(STDERR, "Start the program with a file as argument");
	else if (code == ERR_FILENAME)
		ft_fprintln(STDERR, "The filename should have a .rt extension");
	else if (code == ERR_FILE_CONTENT)
		ft_fprintln(STDERR, "Problem while reading file");
	else if (code == ERR_IDENTIFIER)
		ft_fprintln(STDERR, "Unknown identifier");
	else if (code == ERR_FORMAT)
		ft_fprintln(STDERR, "Wrong format");
	else if (code == ERR_RANGE_DIRECTION)
		ft_fprintln(STDERR, "The direction should have value between -1 and 1");
	else if (code == ERR_RANGE_COLOR)
		ft_fprintln(STDERR, "The color should have value between 0 and 255");
	else if (code == ERR_RANGE_BRIGHTNESS)
		ft_fprintln(STDERR, "The brightness should have a value between 0 and 1");
	else if (code == ERR_RANGE_FOV)
		ft_fprintln(STDERR, "The fov should have a value between 0 and 180");
	else if (code == ERR_FLOAT)
		ft_fprintln(STDERR, "Float expected");
	else if (code == ERR_FORMAT_DIRECTION)
		ft_fprintln(STDERR, "Wrong format of direction");
	else if (code == ERR_FORMAT_COORDINATE)
		ft_fprintln(STDERR, "Wrong format of coordinate");
	else if (code == ERR_FORMAT_COLOR)
		ft_fprintln(STDERR, "Wrong format of color");
	else if (code == ERR_DOUBLE_CAMERA)
		ft_fprintln(STDERR, "There can't be more than one camera");
	else if (code == ERR_DOUBLE_LIGHT)
		ft_fprintln(STDERR, "There can't be more than one light");
	else if (code == ERR_DOUBLE_AMBIENT_LIGHT)
		ft_fprintln(STDERR, "There can't be more than one ambient light");
	if (nb_line)
		ft_fprintln(STDERR, "on line: %d", nb_line);
	if (ft_get_heap_allocations () != 0)
		ft_fprintln (STDERR, "Found %i leaks.", ft_get_heap_allocations ());
	exit(EXIT_FAILURE);
}

int	ft_pow(int nb, int power)
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

t_bool	ft_is_decimal_number(t_cstr str)
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
