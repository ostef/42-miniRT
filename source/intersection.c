/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 16:36:45 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/27 16:36:45 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
	L = vector from ray origin to sphere center
	tca = distance along the ray from the origin to the sphere center
	d = distance from the sphere center to the projection of the sphere center
on the ray
	d2 = d * d
	thc = distance from the projection of the sphere center on the ray, to the
intersection point along the ray
*/
t_bool	ray_sphere_intersection(t_ray ray, t_sphere sph, t_hit_result *res)
{
	t_vec3f	l;
	t_f32	tca;
	t_f32	d2;
	t_f32	dist;

	l = ft_vec3f_sub (sph.center, ray.origin);
	tca = ft_vec3f_dot (l, ray.dir);
	d2 = ft_vec3f_dot (l, l) - tca * tca;
	if (d2 > sph.radius * sph.radius)
		return (FALSE);
	dist = sqrtf (sph.radius * sph.radius - d2);
	if (tca < 0)
		dist = tca + dist;
	else
		dist = tca - dist;
	if (res)
	{
		res->hit = dist >= 0;
		if (res->hit)
		{
			res->point = ft_vec3f_add (ray.origin, ft_vec3f_mulf (ray.dir, dist));
			res->normal = ft_vec3f_normalized (ft_vec3f_sub (res->point, sph.center));
			res->dist = dist;
		}
	}
	return (dist >= 0);
}
