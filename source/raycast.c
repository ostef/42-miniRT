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

t_hit_result	raycast_first(t_rt *rt, t_ray ray)
{
	t_hit_result	hit;
	t_s64			i;

	ft_memset (&hit, 0, sizeof (hit));
	i = 0;
	while (i < rt->obj_count)
	{
		if (ray_object_intersection (ray, &rt->objs[i], &hit))
			return (hit);
		i += 1;
	}
	return (hit);
}

t_hit_result	raycast_first_except(t_rt *rt, t_ray ray, t_object *ignore)
{
	t_hit_result	hit;
	t_s64			i;

	ft_memset (&hit, 0, sizeof (hit));
	i = 0;
	while (i < rt->obj_count)
	{
		if (&rt->objs[i] == ignore)
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

t_hit_result	raycast_closest(t_rt *rt, t_ray ray)
{
	t_hit_result	hit;
	t_hit_result	closest;
	t_s64			i;

	ft_memset (&hit, 0, sizeof (hit));
	ft_memset (&closest, 0, sizeof (closest));
	closest.dist = ft_inf32();
	i = 0;
	while (i < rt->obj_count)
	{
		if (ray_object_intersection (ray, &rt->objs[i], &hit)
			&& hit.dist < closest.dist)
			closest = hit;
		i += 1;
	}
	return (closest);
}
