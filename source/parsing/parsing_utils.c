/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:06:29 by ljourand          #+#    #+#             */
/*   Updated: 2022/10/03 14:23:33 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_str	ft_dup_split_str(t_pcstr split_str, t_alloc arena)
{
	return (ft_strndup(split_str.data, split_str.len, arena));
}

static void	ft_error_2(t_error code)
{
	if (code == ERR_RANGE_DIRECTION)
		ft_fprintln(STDERR, "The direction should have value between -1 and 1");
	else if (code == ERR_RANGE_COLOR)
		ft_fprintln(STDERR, "The color should have values between 0 and 255");
	else if (code == ERR_RANGE_BRIGHTNESS)
		ft_fprintln(STDERR, "The brightness should be a value between 0 and 1");
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
}

void	ft_error(t_error code, t_uint nb_line)
{
	if (code == ERR_OK)
		return ;
	ft_fprintln (STDERR, "Error");
	if (code == ERR_NO_FILENAME)
		ft_fprintln(STDERR, "Start the program with a file as argument");
	else if (code == ERR_FILENAME)
		ft_fprintln(STDERR, "The filename should have a .rt extension");
	else if (code == ERR_FILE_CONTENT)
		ft_fprintln(STDERR, "Problem while reading file");
	else if (code == ERR_NO_CAMERA)
		ft_fprintln(STDERR, "There should be a camera");
	else if (code == ERR_NO_AMBIENT)
		ft_fprintln(STDERR, "There should be an ambient light");
	else if (code == ERR_NO_LIGHT)
		ft_fprintln(STDERR, "There should be a light");
	else if (code == ERR_IDENTIFIER)
		ft_fprintln(STDERR, "Unknown identifier");
	else if (code == ERR_FORMAT)
		ft_fprintln(STDERR, "Wrong format");
	ft_error_2(code);
	if (nb_line)
		ft_fprintln(STDERR, "on line: %d", nb_line);
	if (ft_get_heap_allocations () != 0)
		ft_fprintln (STDERR, "Found %i leaks.", ft_get_heap_allocations ());
	exit(EXIT_FAILURE);
}

t_uint	get_len_split(t_pcstr *split)
{
	t_uint	length;

	length = 0;
	while (split[length].data)
		length++;
	return (length);
}
