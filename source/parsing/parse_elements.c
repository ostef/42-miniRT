/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 17:19:42 by ljourand          #+#    #+#             */
/*   Updated: 2022/10/03 14:10:53 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_error	parse_camera(t_pcstr *split, t_camera *cam, t_alloc ar)
{
	static t_bool	first = TRUE;
	t_vec3f			direction;
	t_error			code;

	if (!first)
		return (ERR_DOUBLE_CAMERA);
	first = FALSE;
	if (get_len_split(split) != 4)
		return (ERR_FORMAT);
	code = parse_coordinate(ft_dup_split_str(split[1], ar), &cam->position, ar);
	if (code != ERR_OK)
		return (code);
	code = parse_direction(ft_dup_split_str(split[2], ar), &direction, ar);
	if (code != ERR_OK)
		return (code);
	cam->yaw = atan2f(direction.y, direction.x) * 180 / PI;
	cam->pitch = asinf(-direction.y) * 180 / PI;
	code = parse_float(ft_dup_split_str(split[3], ar), &cam->fov_in_degrees);
	if (code != ERR_OK)
		return (code);
	if (cam->fov_in_degrees < 0 || cam->fov_in_degrees > 180)
		return (ERR_RANGE_FOV);
	cam->transform = ft_mat4f_identity();
	return (ERR_OK);
}

t_error	parse_ambient_light(t_pcstr *split, t_vec4f *light, t_alloc arena)
{
	static t_bool	first = TRUE;
	t_error			code;

	if (!first)
		return (ERR_DOUBLE_AMBIENT_LIGHT);
	first = FALSE;
	if (get_len_split(split) != 3)
		return (ERR_FORMAT);
	code = parse_float(ft_dup_split_str(split[1], arena), &light->w);
	if (code != ERR_OK)
		return (code);
	if (light->w < 0 || light->w > 1)
		return (ERR_RANGE_BRIGHTNESS);
	code = parse_color(ft_dup_split_str(split[2], arena), &light->rgb, arena);
	return (code);
}

t_error	parse_light(t_pcstr *split, t_rt *rt, t_alloc arena)
{
	static t_bool	first = TRUE;
	t_error			code;
	t_vec3f			position;
	t_vec4f			color;

	if (!first)
		return (ERR_DOUBLE_LIGHT);
	first = FALSE;
	if (get_len_split(split) != 4)
		return (ERR_FORMAT);
	code = parse_coordinate(ft_dup_split_str(split[1], arena),
			&position, arena);
	if (code != ERR_OK)
		return (code);
	code = parse_float(ft_dup_split_str(split[2], arena), &color.w);
	if (code != ERR_OK)
		return (code);
	if (color.w > 1 || color.w < 0)
		return (ERR_RANGE_BRIGHTNESS);
	code = parse_color(ft_dup_split_str(split[3], arena),
			&color.rgb, arena);
	add_light(rt, position, color);
	return (code);
}
