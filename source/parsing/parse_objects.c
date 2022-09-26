/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 17:17:25 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/26 18:29:03 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_error	parse_sphere(t_pcstr *split, t_rt *rt, t_alloc arena)
{
	t_object	*object;
	t_error		code;

	if (get_len_split(split) != 4)
		return (ERR_FORMAT);
	object = add_object(rt);
	object->shape = SPHERE;
	code = parse_coordinate(ft_dup_split_str(split[1], arena),
			&object->sphere.center, arena);
	if (code != ERR_OK)
		return (code);
	code = parse_float(ft_dup_split_str(split[2], arena),
			&object->sphere.radius);
	if (code != ERR_OK)
		return (code);
	object->sphere.radius /= 2;
	code = parse_color(ft_dup_split_str(split[3], arena),
			&object->color.rgb, arena);
	if (code != ERR_OK)
		return (code);
	return (ERR_OK);
}

t_error	parse_plane(t_pcstr *split, t_rt *rt, t_alloc arena)
{
	t_object	*object;
	t_error		code;

	if (get_len_split(split) != 4)
		return (ERR_FORMAT);
	object = add_object(rt);
	object->shape = PLANE;
	code = parse_coordinate(ft_dup_split_str(split[1], arena),
			&object->plane.origin, arena);
	if (code != ERR_OK)
		return (code);
	code = parse_direction(ft_dup_split_str(split[2], arena),
			&object->plane.normal, arena);
	if (code != ERR_OK)
		return (code);
	code = parse_color(ft_dup_split_str(split[3], arena),
			&object->color.rgb, arena);
	return (code);
}

void	get_cylinder(t_cylinder *cylinder, t_f32 height,
		t_vec3f direction, t_vec3f coord)
{
	cylinder->radius = cylinder->radius / 2;
	cylinder->top = ft_vec3f_add(coord,
			ft_vec3f_mulf(direction, height / 2));
	cylinder->bottom = ft_vec3f_sub(coord,
			ft_vec3f_mulf(direction, height / 2));
}

t_error	parse_cylinder(t_pcstr *split, t_rt *rt, t_alloc ar)
{
	t_object	*obj;
	t_vec3f		coord;
	t_vec3f		direction;
	t_f32		height;
	t_error		code;

	if (get_len_split(split) != 6)
		return (ERR_FORMAT);
	obj = add_object(rt);
	obj->shape = CYLINDER;
	code = parse_coordinate(ft_dup_split_str(split[1], ar), &coord, ar);
	if (code != ERR_OK)
		return (code);
	code = parse_direction(ft_dup_split_str(split[2], ar), &direction, ar);
	if (code != ERR_OK)
		return (code);
	code = parse_float(ft_dup_split_str(split[3], ar), &obj->cylinder.radius);
	if (code != ERR_OK)
		return (code);
	code = parse_float(ft_dup_split_str(split[4], ar), &height);
	if (code != ERR_OK)
		return (code);
	get_cylinder(&obj->cylinder, height, direction, coord);
	code = parse_color(ft_dup_split_str(split[5], ar), &obj->color.rgb, ar);
	return (code);
}
