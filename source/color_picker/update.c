/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 13:36:16 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/29 13:36:16 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec4f	update_color_picker(t_rt *rt, t_vec4f color, t_bool has_alpha)
{
	t_vec2f	mouse_pos;
	t_vec2f	center;
	t_vec2f	radius;
	t_rectf	rect;
	t_rectf	alpha_rect;
	t_vec3f	hsv;

	rect = ft_rectf (10, 10,
		rt->win.frame_width * COLOR_PICKER_SCREEN_X_PERCENT,
		rt->win.frame_width * COLOR_PICKER_SCREEN_X_PERCENT);
	center = ft_vec2f_add (rect.pos, ft_vec2f (rect.w * 0.5f, rect.w * 0.5f));
	radius.y = rect.w * 0.5f - 10;
	radius.x = radius.y * COLOR_PICKER_RING_INNER_RADIUS_PERCENT;
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
		rt->color_picker.color.rgb = hsv_to_rgb (hsv);
	}
	else if (rt->active_ui_elem == UI_COLOR_TRIANGLE)
	{
		t_vec2f	sv = sv_from_triangle_point (ft_vec2f ((mouse_pos.x - center.x) / radius.x, (mouse_pos.y - center.y) / radius.x));
		hsv.yz = sv;
		rt->color_picker.color.rgb = hsv_to_rgb (hsv);
	}
	else if (rt->active_ui_elem == UI_COLOR_ALPHA)
		color.a = ft_clampf ((mouse_pos.x - alpha_rect.x) / alpha_rect.w, 0, 1);
	rt->color_picker.color.a = color.a;
	return (rt->color_picker.color);
}
