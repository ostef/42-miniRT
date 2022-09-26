/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:03:25 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/26 18:10:26 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_error	parse_coordinate(t_str str, t_vec3f *result, t_alloc arena)
{
	t_pcstr	*split;
	t_str	x;
	t_str	y;
	t_str	z;

	split = ft_split(str, ',', arena);
	if (get_len_split(split) != 3)
		return (ERR_FORMAT_COORDINATE);
	x = ft_strndup(split[0].data, split[0].len, arena);
	y = ft_strndup(split[1].data, split[1].len, arena);
	z = ft_strndup(split[2].data, split[2].len, arena);
	*result = ft_vec3f(ft_atof(x), ft_atof(y), ft_atof(z));
	if (result->x == NAN || result->y == NAN || result->z == NAN)
		return (ERR_FLOAT);
	return (ERR_OK);
}

t_error	parse_direction(t_str str, t_vec3f *result, t_alloc arena)
{
	t_pcstr	*split;
	t_str	x;
	t_str	y;
	t_str	z;

	split = ft_split(str, ',', arena);
	if (get_len_split(split) != 3)
		return (ERR_FORMAT_DIRECTION);
	x = ft_strndup(split[0].data, split[0].len, arena);
	y = ft_strndup(split[1].data, split[1].len, arena);
	z = ft_strndup(split[2].data, split[2].len, arena);
	*result = ft_vec3f_normalized(ft_vec3f(ft_atof(x), ft_atof(y), ft_atof(z)));
	if (result->x == NAN || result->y == NAN || result->z == NAN)
		return (ERR_FLOAT);
	if (result->x > 1 || result->x < -1 || result->y > 1 || result->y < -1
		|| result->z > 1 || result->z < -1)
		return (ERR_RANGE_DIRECTION);
	return (ERR_OK);
}

t_error	parse_float(t_str str, t_f32 *result)
{
	*result = ft_atof(str);
	if (isnan(*result))
		return (ERR_FLOAT);
	return (ERR_OK);
}

t_error	parse_color(t_str str, t_vec3f *color, t_alloc arena)
{
	t_pcstr	*split;
	t_str	r_str;
	t_str	g_str;
	t_str	b_str;

	split = ft_split(str, ',', arena);
	if (get_len_split(split) != 3)
		return (ERR_FORMAT_COLOR);
	r_str = ft_strndup(split[0].data, split[0].len, arena);
	g_str = ft_strndup(split[1].data, split[1].len, arena);
	b_str = ft_strndup(split[2].data, split[2].len, arena);
	color->r = ft_atof(r_str) / 255;
	color->g = ft_atof(g_str) / 255;
	color->b = ft_atof(b_str) / 255;
	if (color->x == NAN || color->y == NAN || color->z == NAN)
		return (ERR_FLOAT);
	if (color->r < 0 || color->g < 0 || color->b < 0 || color->r > 255
		|| color->g > 255 || color->b > 255)
		return (ERR_RANGE_COLOR);
	return (ERR_OK);
}
