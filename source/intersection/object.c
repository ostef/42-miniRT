/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 14:39:55 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/24 14:39:55 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_bool	ray_object_intersection(t_ray ray, t_object *obj, t_hit_res *res)
{
	t_bool	hit;

	hit = FALSE;
	if (obj->type == SPHERE)
		hit = ray_sphere_intersection (ray, obj->sphere, res);
	else if (obj->type == CYLINDER)
		hit = ray_cylinder_intersection (ray, obj->cylinder, res);
	else if (obj->type == PLANE)
		hit = ray_plane_intersection (ray, obj->plane, res);
	if (hit)
		res->object = obj;
	return (hit);
}
