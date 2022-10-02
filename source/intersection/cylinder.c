/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 19:24:56 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/30 19:24:56 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static t_hit_res	ray_disk_intersection(t_ray ray, t_plane pla, t_f32 rad)
{
	t_hit_res	res;

	if (!ray_plane_intersection (ray, pla, &res))
		return (res);
	if (ft_vec3f_sqrd_dist (res.point, pla.origin) >= rad * rad)
	{
		res.hit = FALSE;
		return (res);
	}
	return (res);
}

/*
	http://pastebin.com/2XrrNcxb
	https://gist.github.com/jdryg/ecde24d34aa0ce2d4d87

Substituting equ. (1) - (6) to equ. (I) and solving for t' gives:
	t' = (t * dot(AB, d) + dot(AB, AO)) / dot(AB, AB); (7) or
	t' = t * m + n where 
	m = dot(AB, d) / dot(AB, AB) and 
	n = dot(AB, AO) / dot(AB, AB)

Substituting (7) into (II) and solving for t gives:
	
	dot(Q, Q)*t^2 + 2*dot(Q, R)*t + (dot(R, R) - r^2) = 0
	where
	Q = d - AB * m
	R = AO - AB * n
*/
t_bool	ray_cylinder_intersection(t_ray ray, t_cylinder cyl, t_hit_res *res)
{
	t_vec3f	ab = ft_vec3f_sub (cyl.top, cyl.bottom);
	t_vec3f	ao = ft_vec3f_sub (ray.origin, cyl.bottom);

	t_f32	ab_dot_d = ft_vec3f_dot (ab, ray.dir);
	t_f32	ab_dot_ao = ft_vec3f_dot (ab, ao);
	t_f32	ab_dot_ab = ft_vec3f_dot (ab, ab);

	t_f32	m = ab_dot_d / ab_dot_ab;
	t_f32	n = ab_dot_ao / ab_dot_ab;

	t_vec3f	q = ft_vec3f_sub (ray.dir, ft_vec3f_mulf (ab, m));
	t_vec3f	r = ft_vec3f_sub (ao, ft_vec3f_mulf (ab, n));

	t_f32	a = ft_vec3f_dot (q, q);
	t_f32	b = 2 * ft_vec3f_dot (q, r);
	t_f32	c = ft_vec3f_dot (r, r) - (cyl.radius * cyl.radius);

	if (a == 0)
	{

		t_plane	bot;
		bot.origin = cyl.bottom;
		bot.normal = ft_vec3f_normalized (ft_vec3f_neg (ab));
		*res = ray_disk_intersection (ray, bot, cyl.radius);

		t_plane	top;
		top.origin = cyl.top;
		top.normal = ft_vec3f_neg (bot.normal);
		t_hit_res	temp_hit = ray_disk_intersection (ray, top, cyl.radius);

		if (temp_hit.hit && (!res->hit || temp_hit.dist < res->dist))
			*res = temp_hit;
		return (res->hit);
	}

	t_f32	discr = b * b - 4 * a * c;	// Polynomial
	if (discr < 0)
	{
		res->hit = FALSE;
		return (FALSE);
	}

	t_f32	sqrt_discr = sqrtf (discr);
	t_f32	tmin = (-b - sqrt_discr) / (2 * a);
	t_f32	tmax = (-b + sqrt_discr) / (2 * a);
	if (tmin > tmax)
	{
		t_f32	tmp = tmin;
		tmin = tmax;
		tmax = tmp;
	}

	t_f32	t_k1 = tmin * m + n;
	t_f32	t_k2 = tmax * m + n;
	t_f32	t_k = (ft_vec3f_dot (ft_vec3f_sub (ray.origin, cyl.bottom), ab) / ft_vec3f_sqrd_len (ab));
	t_bool	is_inside = tmin < 0 && t_k >= 0 && t_k <= 1;
	if (is_inside)
	{
		tmin = tmax;
		t_k1 = t_k2;
	}
	if (t_k1 < 0)	// Bottom plane
	{
		t_plane	bot;
		bot.origin = cyl.bottom;
		bot.normal = ft_vec3f_normalized (ft_vec3f_neg (ab));
		*res = ray_disk_intersection (ray, bot, cyl.radius);
		return (res->hit);
	}
	else if (t_k1 > 1)	// Top plane
	{
		t_plane	top;
		top.origin = cyl.top;
		top.normal = ft_vec3f_normalized (ab);
		*res = ray_disk_intersection (ray, top, cyl.radius);
		return (res->hit);
	}
	else	// Body
	{
		res->hit = tmin > 0 && tmin < ray.length;
		if (res->hit)
		{
			res->point = ft_vec3f_add (ray.origin, ft_vec3f_mulf (ray.dir, tmin));
			res->dist = tmin;
			res->normal = ft_vec3f_direction (ft_vec3f_add (cyl.bottom, ft_vec3f_mulf (ab, t_k1)), res->point);
			if (is_inside)
				res->normal = ft_vec3f_neg (res->normal);
		}
		return (res->hit);
	}

	//t_f32	t_k2 = tmax * m + n;
	// Same as above
	res->hit = FALSE;
	return (FALSE);
}
