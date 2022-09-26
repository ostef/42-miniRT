/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:03:25 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/23 15:22:23 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// TODO check str before sending to atof

t_error	parse_coordinate_f(t_str str, t_vec3f *result)
{
	t_pcstr	*split;
	t_str	x;
	t_str	y;
	t_str	z;

	split = ft_split(str, ',', ft_heap());
	if (!split[0].data || !split[1].data || !split[2].data || split[3].data)
	{
		ft_free(split, ft_heap());
		ft_free(str, ft_heap());
		return (ERR_FORMAT_COORDINATE);
	}
	x = ft_strndup(split[0].data, split[0].len, ft_heap());
	y = ft_strndup(split[1].data, split[1].len, ft_heap());
	z = ft_strndup(split[2].data, split[2].len, ft_heap());
	ft_free(split, ft_heap());
	ft_free(str, ft_heap());
	*result = ft_vec3f(ft_atof(x), ft_atof(y), ft_atof(z));
	ft_free(x, ft_heap());
	ft_free(y, ft_heap());
	ft_free(z, ft_heap());
	if (result->x == NAN || result->y == NAN || result->z == NAN)
		return (ERR_FLOAT);
	return (ERR_OK);
}

t_error	parse_direction_f(t_str str, t_vec3f *result)
{
	t_pcstr	*split;
	t_str	x;
	t_str	y;
	t_str	z;

	split = ft_split(str, ',', ft_heap());
	if (!split[0].data || !split[1].data || !split[2].data || split[3].data)
	{
		ft_free(split, ft_heap());
		ft_free(str, ft_heap());
		return (ERR_FORMAT_DIRECTION);
	}
	x = ft_strndup(split[0].data, split[0].len, ft_heap());
	y = ft_strndup(split[1].data, split[1].len, ft_heap());
	z = ft_strndup(split[2].data, split[2].len, ft_heap());
	ft_free(split, ft_heap());
	ft_free(str, ft_heap());
	*result = ft_vec3f_normalized(ft_vec3f(ft_atof(x), ft_atof(y), ft_atof(z)));
	ft_free(x, ft_heap());
	ft_free(y, ft_heap());
	ft_free(z, ft_heap());
	if (result->x == NAN || result->y == NAN || result->z == NAN)
		return (ERR_FLOAT);
	if (result->x > 1 || result->x < -1 || result->y > 1 || result->y < -1
		|| result->z > 1 || result->z < -1)
		return (ERR_RANGE_DIRECTION);
	return (ERR_OK);
}

t_error	parse_float_f(t_str str, t_f32 *result)
{
	*result = ft_atof(str);
	ft_free(str, ft_heap());
	if (isnan(*result))
		return (ERR_FLOAT);
	return (ERR_OK);
}

t_error	parse_color_f(t_str str, t_vec3f *color)
{
	t_pcstr	*split;
	t_str	r_str;
	t_str	g_str;
	t_str	b_str;

	split = ft_split(str, ',', ft_heap());
	if (!split[0].data || !split[1].data || !split[2].data || split[3].data)
	{
		ft_free(split, ft_heap());
		ft_free(str, ft_heap());
		return (ERR_FORMAT_COLOR);
	}	
	r_str = ft_strndup(split[0].data, split[0].len, ft_heap());
	g_str = ft_strndup(split[1].data, split[1].len, ft_heap());
	b_str = ft_strndup(split[2].data, split[2].len, ft_heap());
	ft_free(split, ft_heap());
	ft_free(str, ft_heap());
	color->r = ft_atof(r_str) / 255;
	color->g = ft_atof(g_str) / 255;
	color->b = ft_atof(b_str) / 255;
	ft_free(r_str, ft_heap());
	ft_free(g_str, ft_heap());
	ft_free(b_str, ft_heap());
	if (color->x == NAN || color->y == NAN || color->z == NAN)
		return (ERR_FLOAT);
	if (color->r < 0 || color->g < 0 || color->b < 0 || color->r > 255
		|| color->g > 255 || color->b > 255)
		return (ERR_RANGE_COLOR);
	return (ERR_OK);
}
