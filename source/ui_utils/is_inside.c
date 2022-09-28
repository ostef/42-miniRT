/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_inside.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 13:08:06 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/28 14:20:30 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_bool	point_is_inside_circle(t_vec2f pt, t_vec2f center, t_f32 radius)
{
	return (ft_vec2f_sqrd_dist (pt, center) < radius * radius);
}

t_bool	point_is_inside_ring(t_vec2f pt, t_vec2f center, t_vec2f radius)
{
	t_f32	sqrd_dist;
	t_vec2f	sqrd_rads;

	sqrd_dist = ft_vec2f_sqrd_dist (pt, center);
	sqrd_rads = ft_vec2f (radius.x * radius.x, radius.y * radius.y);
	return (sqrd_dist > sqrd_rads.x && sqrd_dist < sqrd_rads.y);
}

t_bool	point_is_inside_rect(t_vec2f p, t_rectf r)
{
	return (p.x >= r.x && p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h);
}

static float	_sign(t_vec2f p0, t_vec2f p1, t_vec2f p2)
{
	return ((p0.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p0.y - p2.y));
}

t_bool	point_is_inside_triangle(t_vec2f p, t_vec2f t0, t_vec2f t1, t_vec2f t2)
{
	t_vec3f	d;
	t_bool	has_neg;
	t_bool	has_pos;

	d.x = _sign (p, t0, t1);
	d.y = _sign (p, t1, t2);
	d.z = _sign (p, t2, t0);
	has_neg = (d.x < 0) || (d.y < 0) || (d.z < 0);
	has_pos = (d.x > 0) || (d.y > 0) || (d.z > 0);
	return (!(has_neg && has_pos));
}

