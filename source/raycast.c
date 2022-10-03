/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 17:24:59 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/30 17:24:59 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_hit_res	raycast_first(t_rt *rt, t_ray ray, t_filter fil)
{
	t_hit_res	hit;
	t_s64		i;

	ft_memset (&hit, 0, sizeof (hit));
	i = 0;
	while (i < rt->obj_count)
	{
		if ((fil & (1 << rt->objs[i].type)) == 0)
		{
			i += 1;
			continue ;
		}
		if (ray_object_intersection (ray, &rt->objs[i], &hit))
			return (hit);
		i += 1;
	}
	return (hit);
}

t_hit_res	raycast_closest(t_rt *rt, t_ray ray, t_filter fil)
{
	t_hit_res	hit;
	t_hit_res	closest;
	t_s64		i;

	ft_memset (&hit, 0, sizeof (hit));
	ft_memset (&closest, 0, sizeof (closest));
	closest.dist = ft_inf32();
	i = 0;
	while (i < rt->obj_count)
	{
		if ((fil & (1 << rt->objs[i].type)) == 0)
		{
			i += 1;
			continue ;
		}
		if (ray_object_intersection (ray, &rt->objs[i], &hit)
			&& hit.dist < closest.dist)
			closest = hit;
		i += 1;
	}
	return (closest);
}
