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
#define RING_INNER_RADIUS_PERCENT 0.7f

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

static t_vec2f	sv_from_triangle(t_vec2f p)
{
	t_vec2f	res;

	res.x = ft_clampf ((1 - 2 * p.y) / (sqrtf (3) * p.x - p.y + 2), 0.0001f, 1);
	res.y = ft_clampf ((sqrtf (3) * p.x - p.y + 2) / 3, 0.0001f, 1);
	return (res);
}

static t_vec2f	triangle_point_from_sv(t_vec2f sv)
{
	t_vec2f	res;

	res.x = ((6 * sv.x * sv.y - 3 * sv.x * sv.x * sv.y + sv.x) / (2 * sv.x) - 2) / sqrtf (3);
	res.y = -(3 * sv.y * sv.x - 1) / 2;
	return (res);
}

t_vec4f	update_color_picker(t_rt *rt, t_vec4f color, t_bool has_alpha)
{
	t_vec2f	mouse_pos;
	t_vec2f	center;
	t_vec2f	radius;
	t_rectf	rect;
	t_rectf	alpha_rect;
	t_vec3f	hsv;

	rect = ft_rectf (10, 10,
		rt->win.frame_width * SCREEN_X_PERCENT,
		rt->win.frame_width * SCREEN_X_PERCENT);
	center = ft_vec2f_add (rect.pos, ft_vec2f (rect.w * 0.5f, rect.w * 0.5f));
	radius.y = rect.w * 0.5f - 10;
	radius.x = radius.y * RING_INNER_RADIUS_PERCENT;
	if (has_alpha)
	{
		alpha_rect = ft_rectf (rect.x + 10, rect.y + rect.h, rect.w - 20, rect.h * 0.05f);
		rect.h += alpha_rect.h + 10;
	}
	t_vec2f	p0 = ft_vec2f (radius.x, 0);
	t_vec2f	p1 = ft_vec2f_rotate (p0, 2 * PI / 3);
	t_vec2f	p2 = ft_vec2f_rotate (p1, 2 * PI / 3);
	p0 = ft_vec2f_add (p0, center);
	p1 = ft_vec2f_add (p1, center);
	p2 = ft_vec2f_add (p2, center);

	rt->color_picker.opened = TRUE;
	rt->color_picker.has_alpha = has_alpha;
	rt->color_picker.color = color;
	hsv = rgb_to_hsv (color.rgb);
	mouse_pos = get_mouse_pos (&rt->win);
	if (point_is_inside_rect (mouse_pos, rect))
		rt->ui_captured_mouse = TRUE;
	if (is_key_down (&rt->win, MOUSE_LEFT))
	{
		if (rt->active_ui_elem == UI_COLOR_RING || (rt->active_ui_elem == UI_NONE && point_is_inside_ring (mouse_pos, center, radius)))
		{
			rt->active_ui_elem = UI_COLOR_RING;
			rt->keep_ui_elem_active = TRUE;
		}
		else if (rt->active_ui_elem == UI_COLOR_TRIANGLE || (rt->active_ui_elem == UI_NONE && point_is_inside_triangle (mouse_pos, p0, p1, p2)))
		{
			rt->active_ui_elem = UI_COLOR_TRIANGLE;
			rt->keep_ui_elem_active = TRUE;
		}
		else if (has_alpha && (rt->active_ui_elem == UI_COLOR_ALPHA || (rt->active_ui_elem == UI_NONE && point_is_inside_rect (mouse_pos, alpha_rect))))
		{
			rt->active_ui_elem = UI_COLOR_ALPHA;
			rt->keep_ui_elem_active = TRUE;
		}
	}
	if (rt->active_ui_elem == UI_COLOR_RING)
	{	
		hsv.h = atan2f (mouse_pos.y - center.y, mouse_pos.x - center.x) * 180 / PI;
	}
	else if (rt->active_ui_elem == UI_COLOR_TRIANGLE)
	{
		t_vec2f	sv = sv_from_triangle (ft_vec2f ((mouse_pos.x - center.x) / radius.x, (mouse_pos.y - center.y) / radius.x));
		hsv.s = sv.x;
		hsv.v = sv.y;
	}
	else if (rt->active_ui_elem == UI_COLOR_ALPHA)
		color.a = ft_clampf ((mouse_pos.x - alpha_rect.x) / alpha_rect.w, 0, 1);
	rt->color_picker.color.rgb = hsv_to_rgb (hsv);
	rt->color_picker.color.a = color.a;
	return (rt->color_picker.color);
}

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
				hsv.yz = sv_from_triangle (p);
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
		rt->win.frame_width * SCREEN_X_PERCENT,
		rt->win.frame_width * SCREEN_X_PERCENT);
	center = ft_vec2f_add (rect.pos, ft_vec2f (rect.w * 0.5f, rect.w * 0.5f));
	radius.y = rect.w * 0.5f - 10;
	radius.x = radius.y * RING_INNER_RADIUS_PERCENT;
	if (rt->color_picker.has_alpha)
	{
		alpha_rect = ft_rectf (rect.x + 10, rect.y + rect.h, rect.w - 20, rect.h * 0.07f);
		rect.h += alpha_rect.h + 10;
		alpha = rt->color_picker.color.a;
		alpha_knob_rect = ft_rectf (alpha_rect.x + alpha_rect.w * alpha, alpha_rect.y, alpha_rect.w * 0.04f, alpha_rect.h);
	}

	draw_rect (rt, rect, ft_vec4f (0.2, 0.2, 0.2, 0.8));
	draw_hsv_ring (rt, center, radius);
	t_vec3f	hsv = rgb_to_hsv (rt->color_picker.color.rgb);
	t_vec2f	ring_point = ft_vec2f (cosf (hsv.x * PI / 180), sinf (hsv.x * PI / 180));
	ring_point = ft_vec2f_mulf (ring_point, (radius.x + radius.y) * 0.5f);
	ring_point = ft_vec2f_add (center, ring_point);
	draw_ring (rt, ring_point, ft_vec2f (10, 11), ft_vec4f (1, 1, 1, 1));
	draw_saturation_value_triangle (rt, center, radius.x, rt->color_picker.color.rgb);
	draw_ring (rt, ft_vec2f_add (center, ft_vec2f_mulf (triangle_point_from_sv (hsv.yz), radius.x)), ft_vec2f (10, 11), ft_vec4f (1, 1, 1, 1));
	if (rt->color_picker.has_alpha)
	{
		draw_rect (rt, alpha_rect, ft_vec4f (0.3, 0.3, 0.3, 0.5));
		draw_rect (rt, alpha_knob_rect, ft_vec4f (alpha, alpha, alpha, 1));
	}
}
