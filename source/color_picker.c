/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_picker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 20:37:31 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/26 20:37:31 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

#define SCREEN_X_PERCENT 0.25f
#define RING_INNER_RADIUS_PERCENT 0.6f

static t_f32	fract_part(t_f32 x)
{
	return (x - floorf(x));
}

static t_vec3f	hsv_to_rgb(t_vec3f hsv)
{
	t_vec3f	res;

	hsv.x /= 360.0;
	res.x = ft_absf(fract_part(hsv.x + 1) * 6 - 3) - 1;
	res.y = ft_absf(fract_part(hsv.x + 2/3.0) * 6 - 3) - 1;
	res.z = ft_absf(fract_part(hsv.x + 1/3.0) * 6 - 3) - 1;
	res.x = ft_clampf(res.x, 0, 1);
	res.y = ft_clampf(res.y, 0, 1);
	res.z = ft_clampf(res.z, 0, 1);
	res.x = ft_lerp(1, res.x, hsv.y);
	res.y = ft_lerp(1, res.y, hsv.y);
	res.z = ft_lerp(1, res.z, hsv.y);
	res = ft_vec3f_mulf (res, hsv.z);
	return (res);
}

static t_vec3f	rgb_to_hsv(t_vec3f rgb)
{
	t_f32	min = ft_minf (rgb.r, ft_minf (rgb.g, rgb.b));
	t_f32	max = ft_maxf (rgb.r, ft_maxf (rgb.g, rgb.b));
	if (max == 0)
		return (ft_vec3f (0, 0, 0));
	t_f32	v = max;
	t_f32	delta = max - min;
	t_f32	s = delta / max;
	if (s == 0)
		return (ft_vec3f (0, 0, v));
	t_f32	h;
	if (max == rgb.r)
		h = 0 + (43 / 255.0f) * (rgb.g - rgb.b) / delta;
	else if (max == rgb.g)
		h = (85 / 255.0f) + (43 / 255.0f) * (rgb.b - rgb.r) / delta;
	else
		h = (171 / 255.0f) + (43 / 255.0f) * (rgb.r - rgb.g) / delta;
	return (ft_vec3f (h * 360, s, v));
}

t_vec4f	update_color_picker(t_rt *rt, t_vec4f color, t_bool has_alpha)
{
	t_vec2f	mouse_pos;
	t_vec2f	center;
	t_f32	radius;

	center = ft_vec2f_mulf (ft_vec2f_add (ft_vec2f (10, 10), ft_vec2f (10 + rt->win.frame_width * SCREEN_X_PERCENT, 10 + rt->win.frame_width * SCREEN_X_PERCENT)), 0.5f);
	radius = rt->win.frame_width * SCREEN_X_PERCENT * 0.5f - 10;

	rt->color_picker.opened = TRUE;
	rt->color_picker.has_alpha = has_alpha;
	rt->color_picker.color = color;
	mouse_pos = get_mouse_pos (&rt->win);
	if (mouse_pos.x >= 10 && mouse_pos.x <= 10 + rt->win.frame_width * SCREEN_X_PERCENT
		&& mouse_pos.y >= 10 && mouse_pos.y <= 10 + rt->win.frame_width * SCREEN_X_PERCENT)
		rt->ui_captured_mouse = TRUE;
	mouse_pos = ft_vec2f_sub (mouse_pos, center);
	if (is_key_down (&rt->win, MOUSE_LEFT))
	{
		if (rt->active_ui_elem == UI_COLOR_RING
			|| (ft_vec2f_sqrd_len (mouse_pos) > radius * radius * RING_INNER_RADIUS_PERCENT * RING_INNER_RADIUS_PERCENT && ft_vec2f_sqrd_len (mouse_pos) < radius * radius))
		{
			rt->active_ui_elem = UI_COLOR_RING;
			rt->keep_ui_elem_active = TRUE;
		}
	}
	if (rt->active_ui_elem == UI_COLOR_RING)
	{	
		t_f32	degrees = atan2f (mouse_pos.y, mouse_pos.x) * 180 / PI;
		color.rgb = hsv_to_rgb (ft_vec3f (degrees, 1, 0.9));
	}
	rt->color_picker.color = color;
	return (rt->color_picker.color);
}

static void	draw_hsv_circle(t_rt *rt, t_vec2f center, t_f32 radius)
{
	int		x;
	int		y;
	t_f32	r2;
	t_vec4f	color;
	t_vec2f	p;

	color.a = 1;
	r2 = radius * radius;
	y = ft_max ((int)(center.y - radius), 0);
	while (y <= (int)(center.y + radius) && y < rt->win.frame_height)
	{
		x = ft_max ((int)(center.x - radius), 0);
		while (x <= (int)(center.x + radius) && x < rt->win.frame_width)
		{
			p = ft_vec2f (x - center.x, y - center.y);
			if (ft_vec2f_sqrd_len (p) > r2 * RING_INNER_RADIUS_PERCENT * RING_INNER_RADIUS_PERCENT
				&& ft_vec2f_sqrd_len (p) < r2)
			{
				t_f32	degrees = atan2f (p.y, p.x) * 180 / PI;
				color.rgb = hsv_to_rgb (ft_vec3f (degrees, 1, 0.9));
				draw_pixel (rt, x, y, color);
			}
			x += 1;
		}
		y += 1;
	}
}

void	draw_color_picker(t_rt *rt)
{
	t_vec2f	min;
	t_vec2f	max;
	t_vec2f	center;
	t_f32	radius;

	if (!rt->color_picker.opened)
		return ;
	min = ft_vec2f (10, 10);
	max = ft_vec2f (10 + rt->win.frame_width * SCREEN_X_PERCENT, 10 + rt->win.frame_width * SCREEN_X_PERCENT);
	center = ft_vec2f_mulf (ft_vec2f_add (min, max), 0.5f);
	radius = rt->win.frame_width * SCREEN_X_PERCENT * 0.5f - 10;
	draw_rect (rt, min, max, ft_vec4f (0.2, 0.2, 0.2, 0.8));
	draw_hsv_circle (rt, center, radius);
	t_vec3f	hsv = rgb_to_hsv (rt->color_picker.color.rgb);
	t_vec2f	ring_point = ft_vec2f (cosf (hsv.x * PI / 180), sinf (hsv.x * PI / 180));
	draw_ring (rt, ft_vec2f_add (center, ft_vec2f_mulf (ring_point, (radius + radius * RING_INNER_RADIUS_PERCENT) * 0.5f)), ft_vec2f (10, 12), ft_vec4f (1, 1, 1, 1));
}
