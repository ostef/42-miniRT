/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 14:39:36 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/24 14:39:36 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_bool	ray_plane_intersection(t_ray ray, t_plane pla, t_hit_res *res)
{
	t_f32	denom;
	t_f32	t;

	denom = ft_vec3f_dot (pla.normal, ray.dir);
	if (!ft_approx_zero (denom, 0.0001f))
	{
		t = ft_vec3f_dot (ft_vec3f_sub (pla.origin, ray.origin), pla.normal)
			/ denom;
		if (res)
		{
			res->hit = t > 0.0001f && t < ray.length;
			if (res->hit)
			{
				res->dist = t;
				res->point = ft_vec3f_add (ray.origin,
						ft_vec3f_mulf (ray.dir, res->dist));
				res->normal = ft_vec3f_mulf (pla.normal, -ft_signf (denom));
			}
		}
		return (t > 0.0001f && t < ray.length);
	}
	if (res)
		res->hit = FALSE;
	return (FALSE);
}
