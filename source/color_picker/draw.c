/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 13:33:19 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/29 13:33:19 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	draw_hsv_ring(t_rt *rt, t_vec2f center, t_vec2f radius)
{
	int		x;
	int		y;
	t_vec2f	r2;
	t_vec4f	color;

	color.a = 1;
	r2 = ft_vec2f (radius.x * radius.x, radius.y * radius.y);
	y = ft_max ((int)(center.y - radius.y), 0);
	while (y <= (int)(center.y + radius.y) && y < rt->win.frame_height)
	{
		x = ft_max ((int)(center.x - radius.y), 0);
		while (x <= (int)(center.x + radius.y) && x < rt->win.frame_width)
		{
			if (point_is_inside_ring (ft_vec2f (x, y), center, radius))
			{
				color.rgb = hsv_to_rgb (ft_vec3f (
							atan2f(y - center.y, x - center.x) * 180 / PI,
							1, 0.9));
				draw_pixel (rt, x, y, color);
			}
			x += 1;
		}
		y += 1;
	}
}

static void	draw_sv_px(t_rt *rt, int x, int y, t_vec3f base_col)
{
	t_vec2f	center;
	t_f32	radius;
	t_vec3f	hsv;

	center = rt->cp.ring_center;
	radius = rt->cp.ring_radius.x;
	if (point_is_inside_triangle (ft_vec2f (x, y),
			rt->cp.tri[0], rt->cp.tri[1], rt->cp.tri[2]))
	{
		hsv.h = base_col.h;
		hsv.yz = sv_from_triangle_point ((x - center.x) / radius,
				(y - center.y) / radius);
		hsv = hsv_to_rgb (hsv);
		draw_pixel (rt, x, y, ft_vec4f (hsv.r, hsv.g, hsv.b, 1));
	}
}

static void	draw_sv_triangle(t_rt *rt, t_vec2f center,
	t_f32 radius, t_vec3f base_col)
{
	int		x;
	int		y;
	t_vec2f	p[3];

	ft_memcpy (p, rt->cp.tri, sizeof (rt->cp.tri));
	base_col = rgb_to_hsv (base_col);
	y = ft_max ((int)ft_minf (p[0].y, ft_minf (p[1].y, p[2].y)), 0);
	while (y <= (int)ft_maxf (p[0].y, ft_maxf (p[1].y, p[2].y))
		&& y < rt->win.frame_height)
	{
		x = ft_max ((int)ft_minf (p[0].x, ft_minf (p[1].x, p[2].x)), 0);
		while (x <= (int)ft_maxf (p[0].x, ft_maxf (p[1].x, p[2].x))
			&& x < rt->win.frame_width)
		{
			draw_sv_px (rt, x, y, base_col);
			x += 1;
		}
		y += 1;
	}
}

static void	draw_sv_and_alpha(t_rt *rt, t_vec3f hsv)
{
	t_rectf	alpha_knob_rect;
	t_f32	alpha;

	draw_sv_triangle (rt, rt->cp.ring_center, rt->cp.ring_radius.x,
		rt->cp.color.rgb);
	draw_ring (rt, ft_vec2f_add (rt->cp.ring_center,
			ft_vec2f_mulf (triangle_point_from_sv (hsv.s, hsv.v),
				rt->cp.ring_radius.x)),
		ft_vec2f (10, 11), ft_vec4f (1, 1, 1, 1));
	if (rt->cp.has_alpha)
	{
		alpha = rt->cp.color.a;
		alpha_knob_rect = ft_rectf (
				rt->cp.alpha_rect.x + rt->cp.alpha_rect.w * alpha,
				rt->cp.alpha_rect.y,
				rt->cp.alpha_rect.w * 0.04f, rt->cp.alpha_rect.h);
		draw_rect (rt, rt->cp.alpha_rect, ft_vec4f (0.3, 0.3, 0.3, 0.5));
		draw_rect (rt, alpha_knob_rect, ft_vec4f (alpha, alpha, alpha, 1));
	}
}

void	draw_color_picker(t_rt *rt)
{
	t_vec3f	hsv;
	t_vec2f	ring_point;

	if (!rt->cp.opened)
		return ;
	draw_rect (rt, rt->cp.rect, ft_vec4f (0.2, 0.2, 0.2, 0.8));
	draw_hsv_ring (rt, rt->cp.ring_center, rt->cp.ring_radius);
	hsv = rgb_to_hsv (rt->cp.color.rgb);
	ring_point = ft_vec2f (cosf (hsv.x * PI / 180),
			sinf (hsv.x * PI / 180));
	ring_point = ft_vec2f_mulf (ring_point,
			(rt->cp.ring_radius.x + rt->cp.ring_radius.y) * 0.5f);
	ring_point = ft_vec2f_add (rt->cp.ring_center, ring_point);
	draw_ring (rt, ring_point, ft_vec2f (10, 11), ft_vec4f (1, 1, 1, 1));
	draw_sv_and_alpha (rt, hsv);
}
