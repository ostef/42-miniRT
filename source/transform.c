/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:41:05 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/22 15:41:05 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	translate_object(t_object *obj, t_vec3f amount)
{
	if (obj->shape == SPHERE)
		obj->sphere.center = ft_vec3f_add (obj->sphere.center, amount);
	else if (obj->shape == CYLINDER)
	{
		obj->cylinder.bottom = ft_vec3f_add (obj->cylinder.bottom, amount);
		obj->cylinder.top = ft_vec3f_add (obj->cylinder.top, amount);
	}
	else if (obj->shape == PLANE)
		obj->plane.origin = ft_vec3f_add (obj->plane.origin, amount);
}

void	rotate_object(t_object *obj, t_vec3f amount)
{
	t_mat4f	mat;
	t_vec3f	cyl_u;
	t_vec3f	cyl_c;
	t_f32	cyl_h;

	mat = ft_mat4f_rotate_euler (ft_vec3f_mulf (amount, PI / 180.0f));
	if (obj->shape == CYLINDER)
	{
		cyl_u = ft_vec3f_normalized (
				ft_vec3f_sub (obj->cylinder.top, obj->cylinder.bottom));
		cyl_u = ft_mat4f_transform_vector (mat, cyl_u);
		cyl_c = ft_vec3f_mulf (
				ft_vec3f_add (obj->cylinder.bottom, obj->cylinder.top), 0.5f);
		cyl_h = ft_vec3f_dist (obj->cylinder.bottom, obj->cylinder.top);
		obj->cylinder.bottom = ft_vec3f_sub (cyl_c,
				ft_vec3f_mulf (cyl_u, cyl_h * 0.5f));
		obj->cylinder.top = ft_vec3f_add (cyl_c,
				ft_vec3f_mulf (cyl_u, cyl_h * 0.5f));
	}
	else if (obj->shape == PLANE)
	{
		obj->plane.normal = ft_mat4f_transform_vector (mat, obj->plane.normal);
	}
}
