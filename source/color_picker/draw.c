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
	t_vec2f	p;

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
				t_f32	degrees = atan2f (y - center.y, x - center.x) * 180 / PI;
				color.rgb = hsv_to_rgb (ft_vec3f (degrees, 1, 0.9));
				draw_pixel (rt, x, y, color);
			}
			x += 1;
		}
		y += 1;
	}
}

static void	draw_saturation_value_triangle(t_rt *rt, t_vec2f center, t_f32 radius, t_vec3f base_col)
{
	int		x;
	int		y;
	t_vec3f	hsv;
	t_vec2f	p0 = ft_vec2f (0, -radius);
	t_vec2f	p1 = ft_vec2f_rotate (p0, 2 * PI / 3);
	t_vec2f	p2 = ft_vec2f_rotate (p1, 2 * PI / 3);

	base_col = rgb_to_hsv (base_col);
	p0 = ft_vec2f_add (p0, center);
	p1 = ft_vec2f_add (p1, center);
	p2 = ft_vec2f_add (p2, center);
	y = (int)ft_minf (p0.y, ft_minf (p1.y, p2.y));
	y = ft_max (y, 0);
	while (y <= (int)ft_maxf (p0.y, ft_maxf (p1.y, p2.y)) && y < rt->win.frame_height)
	{
		x = (int)ft_minf (p0.x, ft_minf (p1.x, p2.x));
		x = ft_max (x, 0);
		while (x <= (int)ft_maxf (p0.x, ft_maxf (p1.x, p2.x)) && x < rt->win.frame_width)
		{
			if (point_is_inside_triangle (ft_vec2f (x, y), p0, p1, p2))
			{
				t_vec2f	p = ft_vec2f ((x - center.x) / radius, (y - center.y) / radius);
				hsv.h = base_col.h;
				hsv.yz = sv_from_triangle_point (p);
				hsv = hsv_to_rgb (hsv);
				draw_pixel (rt, x, y, ft_vec4f (hsv.r, hsv.g, hsv.b, 1));
			}
			x += 1;
		}
		y += 1;
	}
}

void	draw_color_picker(t_rt *rt)
{
	t_vec2f	center;
	t_vec2f	radius;
	t_rectf	rect;
	t_rectf	alpha_rect;
	t_rectf	alpha_knob_rect;
	t_f32	alpha;

	if (!rt->color_picker.opened)
		return ;
	rect = ft_rectf (10, 10,
		rt->win.frame_width * COLOR_PICKER_SCREEN_X_PERCENT,
		rt->win.frame_width * COLOR_PICKER_SCREEN_X_PERCENT);
	center = ft_vec2f_add (rect.pos, ft_vec2f (rect.w * 0.5f, rect.w * 0.5f));
	radius.y = rect.w * 0.5f - 10;
	radius.x = radius.y * COLOR_PICKER_RING_INNER_RADIUS_PERCENT;
	if (rt->color_picker.has_alpha)
	{
		alpha_rect = ft_rectf (rect.x + 10, rect.y + rect.h,
			rect.w - 20, rect.h * 0.07f);
		rect.h += alpha_rect.h + 10;
		alpha = rt->color_picker.color.a;
		alpha_knob_rect = ft_rectf (alpha_rect.x + alpha_rect.w * alpha,
			alpha_rect.y, alpha_rect.w * 0.04f, alpha_rect.h);
	}

	draw_rect (rt, rect, ft_vec4f (0.2, 0.2, 0.2, 0.8));
	draw_hsv_ring (rt, center, radius);
	t_vec3f	hsv = rgb_to_hsv (rt->color_picker.color.rgb);
	t_vec2f	ring_point = ft_vec2f (cosf (hsv.x * PI / 180),
		sinf (hsv.x * PI / 180));
	ring_point = ft_vec2f_mulf (ring_point, (radius.x + radius.y) * 0.5f);
	ring_point = ft_vec2f_add (center, ring_point);
	draw_ring (rt, ring_point, ft_vec2f (10, 11), ft_vec4f (1, 1, 1, 1));
	draw_saturation_value_triangle (rt, center, radius.x,
		rt->color_picker.color.rgb);
	draw_ring (rt, ft_vec2f_add (center,
		ft_vec2f_mulf (triangle_point_from_sv (hsv.yz), radius.x)),
		ft_vec2f (10, 11), ft_vec4f (1, 1, 1, 1));
	if (rt->color_picker.has_alpha)
	{
		draw_rect (rt, alpha_rect, ft_vec4f (0.3, 0.3, 0.3, 0.5));
		draw_rect (rt, alpha_knob_rect, ft_vec4f (alpha, alpha, alpha, 1));
	}
}
