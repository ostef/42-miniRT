/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pixel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 12:36:58 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/27 12:36:58 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

#ifdef FT_WIN32

void	draw_pixel(t_rt *rt, int x, int y, t_vec4f color)
{
	t_vec4f	px;

	y = rt->win.frame_height - y;
	if (x < 0 || x >= rt->win.frame_height
		|| y < 0 || y >= rt->win.frame_height)
		return ;
	px = get_pixel (&rt->win, x, y);
	px.r = color.r * color.a + px.r * (1 - color.a);
	px.g = color.g * color.a + px.g * (1 - color.a);
	px.b = color.b * color.a + px.b * (1 - color.a);
	set_pixel (&rt->win, x, y, px);
}

#else

void	draw_pixel(t_rt *rt, int x, int y, t_vec4f color)
{
	t_vec4f	px;

	if (x < 0 || x >= rt->win.frame_height
		|| y < 0 || y >= rt->win.frame_height)
		return ;
	px = get_pixel (&rt->win, x, y);
	px.r = color.r * color.a + px.r * (1 - color.a);
	px.g = color.g * color.a + px.g * (1 - color.a);
	px.b = color.b * color.a + px.b * (1 - color.a);
	set_pixel (&rt->win, x, y, px);
}

#endif
