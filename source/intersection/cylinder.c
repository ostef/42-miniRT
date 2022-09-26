/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 14:18:39 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/24 14:18:39 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

typedef struct s_dat
{
	t_vec3f	bot_to_top;
	t_vec3f	bot_to_ori;
	t_f32	sqrd_height;
	t_f32	dir_on_cyl;
	t_f32	up_on_bot_to_ori;
	t_f32	k0;
	t_f32	k1;
	t_f32	k2;
	t_f32	h;
	t_f32	t;
	t_f32	y;
}	t_dat;

static t_bool	ray_cylinder_init(t_ray ray, t_cylinder cyl,
	t_dat *dat, t_hit_res *res)
{
	dat->bot_to_top = ft_vec3f_sub (cyl.top, cyl.bottom);
	dat->bot_to_ori = ft_vec3f_sub (ray.origin, cyl.bottom);
	dat->sqrd_height = ft_vec3f_dot (dat->bot_to_top, dat->bot_to_top);
	dat->dir_on_cyl = ft_vec3f_dot (dat->bot_to_top, ray.dir);
	dat->up_on_bot_to_ori = ft_vec3f_dot (dat->bot_to_top, dat->bot_to_ori);
	dat->k0 = dat->sqrd_height * ft_vec3f_sqrd_len (dat->bot_to_ori)
		- dat->up_on_bot_to_ori * dat->up_on_bot_to_ori
		- cyl.radius * cyl.radius * dat->sqrd_height;
	dat->k1 = dat->sqrd_height * ft_vec3f_dot (dat->bot_to_ori, ray.dir)
		- dat->up_on_bot_to_ori * dat->dir_on_cyl;
	dat->k2 = dat->sqrd_height - dat->dir_on_cyl * dat->dir_on_cyl;
	dat->h = dat->k1 * dat->k1 - dat->k2 * dat->k0;
	if (dat->h < 0)
	{
		if (res)
			res->hit = FALSE;
		return (FALSE);
	}
	dat->h = sqrtf (dat->h);
	dat->t = (-dat->k1 - dat->h) / dat->k2;
	return (TRUE);
}

static t_bool	ray_cylinder_body_intersection(t_ray ray, t_cylinder cyl,
	t_dat *dat, t_hit_res *res)
{
	dat->y = dat->up_on_bot_to_ori + dat->t * dat->dir_on_cyl;
	if (dat->t >= 0 && dat->y > 0 && dat->y < dat->sqrd_height)
	{
		if (res)
		{
			res->dist = dat->t;
			res->normal = ft_vec3f_mulf (ft_vec3f_sub (
						ft_vec3f_add (
							dat->bot_to_ori,
							ft_vec3f_mulf (ray.dir, dat->t)),
						ft_vec3f_mulf (
							dat->bot_to_top,
							dat->y / dat->sqrd_height)),
					1 / cyl.radius);
			res->point = ft_vec3f_add (
					ray.origin,
					ft_vec3f_mulf (ray.dir, dat->t)
					);
			res->hit = TRUE;
		}
		return (TRUE);
	}
	return (FALSE);
}

static t_bool	ray_cylinder_caps_intersection(t_ray ray, t_cylinder cyl,
	t_dat *dat, t_hit_res *res)
{
	if (dat->y < 0)
		dat->t = -dat->up_on_bot_to_ori / dat->dir_on_cyl;
	else
		dat->t = (dat->sqrd_height - dat->up_on_bot_to_ori) / dat->dir_on_cyl;
	if (dat->t >= 0 && ft_absf (dat->k1 + dat->k2 * dat->t) < dat->h)
	{
		if (res)
		{
			res->dist = dat->t;
			res->normal = ft_vec3f_mulf (
					ft_vec3f_normalized (dat->bot_to_top), ft_signf (dat->y));
			res->point = ft_vec3f_add (ray.origin,
					ft_vec3f_mulf (ray.dir, dat->t));
			res->hit = TRUE;
		}
		return (TRUE);
	}
	return (FALSE);
}

t_bool	ray_cylinder_intersection(t_ray ray, t_cylinder cyl, t_hit_res *res)
{
	t_dat	dat;

	if (!ray_cylinder_init (ray, cyl, &dat, res))
		return (FALSE);
	if (ray_cylinder_body_intersection (ray, cyl, &dat, res))
		return (TRUE);
	if (ray_cylinder_caps_intersection (ray, cyl, &dat, res))
		return (TRUE);
	if (res)
		res->hit = FALSE;
	return (FALSE);
}
