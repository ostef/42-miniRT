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
	d[0] = d * d
	d[1] = distance from the sphere center to the projection of the sphere center
on the ray
	thc = distance from tca, to the intersection point along the ray
*/
t_bool	ray_sphere_intersection(t_ray ray, t_sphere sph, t_hit_res *res)
{
	t_vec3f	l;
	t_f32	tca;
	t_f32	d[2];
	t_f32	thc;

	l = ft_vec3f_sub (sph.center, ray.origin);
	tca = ft_vec3f_dot (l, ray.dir);
	d[0] = ft_vec3f_dot (l, l) - tca * tca;
	if (d[0] > sph.radius * sph.radius)
		return (FALSE);
	thc = sqrtf (sph.radius * sph.radius - d[0]);
	if (thc > ft_absf (tca))
		d[1] = tca + thc;
	else
		d[1] = tca - (ft_signf (tca)) * thc;
	if (res)
	{
		res->hit = d[1] >= 0;
		if (res->hit)
		{
			res->point = ft_vec3f_add(ray.origin, ft_vec3f_mulf(ray.dir, d[1]));
			res->normal = ft_vec3f_normalized (
					ft_vec3f_sub (res->point, sph.center));
			if (thc > ft_absf (tca))
				res->normal = ft_vec3f_neg (res->normal);
			res->dist = d[1];
		}
	}
	return (d[1] >= 0);
}
