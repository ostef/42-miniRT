/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 22:32:17 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/25 22:32:17 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_object	*add_object(t_rt *rt)
{
	if (rt->obj_count == rt->obj_cap)
	{
		rt->objs = ft_realloc (rt->objs, rt->obj_cap * sizeof(t_object),
				(rt->obj_cap * 2 + 8) * sizeof(t_object), ft_heap());
		rt->obj_cap = rt->obj_cap * 2 + 8;
	}
	rt->obj_count += 1;
	return (&rt->objs[rt->obj_count - 1]);
}

t_object	*add_sphere(t_rt *rt, t_vec3f center, t_f32 radius)
{
	t_object	*obj;

	obj = add_object(rt);
	obj->shape = SPHERE;
	obj->sphere.center = center;
	obj->sphere.radius = radius;
	return (obj);
}

t_object	*add_cylinder(t_rt *rt, t_vec3f bottom, t_vec3f top, t_f32 radius)
{
	t_object	*obj;

	obj = add_object(rt);
	obj->shape = CYLINDER;
	obj->cylinder.bottom = bottom;
	obj->cylinder.top = top;
	obj->cylinder.radius = radius;
	return (obj);
}

t_object	*add_plane(t_rt *rt, t_vec3f origin, t_vec3f normal)
{
	t_object	*obj;

	obj = add_object(rt);
	obj->shape = PLANE;
	obj->plane.origin = origin;
	obj->plane.normal = ft_vec3f_normalized(normal);
	return (obj);
}

void	remove_object(t_rt *rt, t_s64 index)
{
	ft_assert(index >= 0 && index < rt->obj_count,
		"Invalid obj index %d.", index);
	while (index < rt->obj_count - 1)
	{
		rt->objs[index] = rt->objs[index + 1];
		index += 1;
	}
	rt->obj_count -= 1;
}
