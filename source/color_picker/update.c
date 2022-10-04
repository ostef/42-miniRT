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

static void	init_color_picker(t_rt *rt, t_vec4f color, t_bool has_alpha)
{
	rt->cp.opened = TRUE;
	rt->cp.has_alpha = has_alpha;
	rt->cp.color = color;
	rt->cp.rect = ft_rectf (10, 10,
			rt->win.frame_width * COLOR_PICKER_SCREEN_X_PERCENT,
			rt->win.frame_width * COLOR_PICKER_SCREEN_X_PERCENT);
	rt->cp.ring_center = ft_vec2f_add (rt->cp.rect.pos,
			ft_vec2f (rt->cp.rect.w * 0.5f, rt->cp.rect.w * 0.5f));
	rt->cp.ring_radius.y = rt->cp.rect.w * 0.5f - 10;
	rt->cp.ring_radius.x = rt->cp.ring_radius.y * COLOR_PICKER_RING_INNER_RAD;
	if (rt->cp.has_alpha)
	{
		rt->cp.alpha_rect = ft_rectf (
				rt->cp.rect.x + 10, rt->cp.rect.y + rt->cp.rect.h,
				rt->cp.rect.w - 20, rt->cp.rect.h * 0.05f);
		rt->cp.rect.h += rt->cp.alpha_rect.h + 10;
	}
	rt->cp.tri[0] = ft_vec2f (0, -rt->cp.ring_radius.x);
	rt->cp.tri[1] = ft_vec2f_rotate (rt->cp.tri[0], 2 * PI / 3);
	rt->cp.tri[2] = ft_vec2f_rotate (rt->cp.tri[1], 2 * PI / 3);
	rt->cp.tri[0] = ft_vec2f_add (rt->cp.tri[0], rt->cp.ring_center);
	rt->cp.tri[1] = ft_vec2f_add (rt->cp.tri[1], rt->cp.ring_center);
	rt->cp.tri[2] = ft_vec2f_add (rt->cp.tri[2], rt->cp.ring_center);
}

static t_bool	ui_activate(t_rt *rt, t_ui_id elem, t_bool cond)
{
	if (rt->active_ui_elem == elem || (!rt->active_ui_elem && cond))
	{
		rt->active_ui_elem = elem;
		rt->keep_ui_elem_active = TRUE;
	}
	return (rt->active_ui_elem == elem);
}

static void	update_ui(t_rt *rt, t_bool down, t_vec2f m, t_vec3f hsv)
{
	if (down && ui_activate (rt, UI_COLOR_RING,
			point_is_inside_ring (m,
				rt->cp.ring_center, rt->cp.ring_radius)))
	{
		hsv.h = atan2f(m.y - rt->cp.ring_center.y, m.x - rt->cp.ring_center.x);
		hsv.h *= 180 / PI;
		rt->cp.color.rgb = hsv_to_rgb (hsv);
	}
	else if (down && ui_activate (rt, UI_COLOR_TRIANGLE,
			point_is_inside_triangle (m,
				rt->cp.tri[0], rt->cp.tri[1], rt->cp.tri[2])))
	{
		hsv.yz = sv_from_triangle_point (
				(m.x - rt->cp.ring_center.x) / rt->cp.ring_radius.x,
				(m.y - rt->cp.ring_center.y) / rt->cp.ring_radius.x);
		rt->cp.color.rgb = hsv_to_rgb (hsv);
	}
	else if (down && ui_activate (rt, UI_COLOR_ALPHA,
			rt->cp.has_alpha
			&& point_is_inside_rect (m, rt->cp.alpha_rect)))
	{
		rt->cp.color.a = (m.x - rt->cp.alpha_rect.x) / rt->cp.alpha_rect.w;
		rt->cp.color.a = ft_clampf (rt->cp.color.a, 0, 1);
	}
}

t_vec4f	update_color_picker(t_rt *rt, t_vec4f color, t_bool has_alpha)
{
	t_vec2f	m;
	t_vec3f	hsv;

	init_color_picker (rt, color, has_alpha);
	hsv = rgb_to_hsv (color.rgb);
	m = get_mouse_pos (&rt->win);
	if (point_is_inside_rect (m, rt->cp.rect) || rt->active_ui_elem)
		rt->ui_captured_mouse = TRUE;
	update_ui (rt, is_key_down (&rt->win, MOUSE_LEFT), m, hsv);
	return (rt->cp.color);
}
