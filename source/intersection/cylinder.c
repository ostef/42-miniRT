/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 19:24:56 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/30 19:24:56 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
	http://pastebin.com/2XrrNcxb
	https://gist.github.com/jdryg/ecde24d34aa0ce2d4d87

Substituting equ. (1) - (6) to equ. (I) and solving for t' gives:
	t' = (t * dot(AB, d) + dot(AB, AO)) / dot(AB, AB); (7) or
	t' = t * d.m + d.n where 
	d.m = dot(AB, d) / dot(AB, AB) and 
	d.n = dot(AB, AO) / dot(AB, AB)

Substituting (7) into (II) and solving for t gives:
	
	dot(Q, Q)*t^2 + 2*dot(Q, R)*t + (dot(R, R) - d.r^2) = 0
	where
	Q = d - AB * d.m
	R = AO - AB * d.n
*/

typedef struct s_inter_data
{
	t_vec3f	ab;
	t_vec3f	ao;
	t_f32	ab_dot_d;
	t_f32	ab_dot_ao;
	t_f32	ab_dot_ab;
	t_f32	m;
	t_f32	n;
	t_vec3f	q;
	t_vec3f	r;
	t_f32	a;
	t_f32	b;
	t_f32	c;
	t_f32	discr;
	t_f32	tmin;
	t_f32	tmax;
	t_f32	tk;
	t_bool	is_inside;
}	t_inter_data;

static t_inter_data	ray_cylinder_intersection_init(t_ray ray, t_cylinder cyl)
{
	t_inter_data	d;

	ft_memset (&d, 0, sizeof (d));
	d.ab = ft_vec3f_sub (cyl.top, cyl.bottom);
	d.ao = ft_vec3f_sub (ray.origin, cyl.bottom);
	d.ab_dot_d = ft_vec3f_dot (d.ab, ray.dir);
	d.ab_dot_ao = ft_vec3f_dot (d.ab, d.ao);
	d.ab_dot_ab = ft_vec3f_dot (d.ab, d.ab);
	d.m = d.ab_dot_d / d.ab_dot_ab;
	d.n = d.ab_dot_ao / d.ab_dot_ab;
	d.q = ft_vec3f_sub (ray.dir, ft_vec3f_mulf (d.ab, d.m));
	d.r = ft_vec3f_sub (d.ao, ft_vec3f_mulf (d.ab, d.n));
	d.a = ft_vec3f_dot (d.q, d.q);
	d.b = 2 * ft_vec3f_dot (d.q, d.r);
	d.c = ft_vec3f_dot (d.r, d.r) - (cyl.radius * cyl.radius);
	d.discr = d.b * d.b - 4 * d.a * d.c;
	return (d);
}

static t_bool	ray_cylinder_caps_intersection(t_ray ray, t_cylinder cyl,
	t_inter_data d, t_hit_res *res)
{
	t_plane	pla;

	if (d.tk < 0)
	{
		pla.origin = cyl.bottom;
		pla.normal = ft_vec3f_normalized (ft_vec3f_neg (d.ab));
	}
	else
	{
		pla.origin = cyl.top;
		pla.normal = ft_vec3f_normalized (d.ab);
	}
	if (!ray_plane_intersection (ray, pla, res))
		return (FALSE);
	if (ft_vec3f_sqrd_dist (res->point, pla.origin) > cyl.radius * cyl.radius)
	{
		res->hit = FALSE;
		return (FALSE);
	}
	return (TRUE);
}

static t_bool	ray_cylinder_body_intersection(t_ray ray, t_cylinder cyl,
	t_inter_data d, t_hit_res *res)
{
	res->hit = (d.tmin > 0 && d.tmin < ray.length);
	if (res->hit)
	{
		res->point = ft_vec3f_add(ray.origin, ft_vec3f_mulf(ray.dir, d.tmin));
		res->dist = d.tmin;
		if (d.is_inside)
			res->normal = ft_vec3f_direction (res->point,
					ft_vec3f_add (cyl.bottom, ft_vec3f_mulf (d.ab, d.tk)));
		else
			res->normal = ft_vec3f_direction (
					ft_vec3f_add(cyl.bottom, ft_vec3f_mulf(d.ab, d.tk)),
					res->point);
	}
	return (res->hit);
}

t_bool	ray_cylinder_intersection(t_ray ray, t_cylinder cyl, t_hit_res *res)
{
	t_inter_data	d;
	t_f32			sqrt_discr;

	d = ray_cylinder_intersection_init (ray, cyl);
	if (d.discr < 0)
	{
		res->hit = FALSE;
		return (FALSE);
	}
	sqrt_discr = sqrtf (d.discr);
	d.tmin = (-d.b - sqrt_discr) / (2 * d.a);
	d.tmax = (-d.b + sqrt_discr) / (2 * d.a);
	if (d.tmin > d.tmax)
		d.tmin = d.tmax;
	d.is_inside = (d.tmin < 0 && d.tmax >= 0 && d.n >= 0 && d.n <= 1);
	if (d.is_inside)
		d.tmin = d.tmax;
	d.tk = d.tmin * d.m + d.n;
	if (d.tk < 0 || d.tk > 1)
		return (ray_cylinder_caps_intersection (ray, cyl, d, res));
	return (ray_cylinder_body_intersection (ray, cyl, d, res));
}
