/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_circle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 14:26:23 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/27 14:26:23 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	draw_circle(t_rt *rt, t_vec2f center, t_f32 radius, t_vec4f color)
{
	int		x;
	int		y;
	t_f32	r2;

	r2 = radius * radius;
	y = ft_max ((int)(center.y - radius), 0);
	while (y <= (int)(center.y + radius) && y < rt->win.frame_height)
	{
		x = ft_max ((int)(center.x - radius), 0);
		while (x <= (int)(center.x + radius) && x < rt->win.frame_width)
		{
			if (ft_vec2f_sqrd_len (ft_vec2f (x - center.x, y - center.y)) < r2)
				draw_pixel (rt, x, y, color);
			x += 1;
		}
		y += 1;
	}
}

void	draw_ring(t_rt *rt, t_vec2f center, t_vec2f radius, t_vec4f color)
{
	int		x;
	int		y;
	t_vec2f	r2;
	t_f32	sqrd_len;

	r2.x = radius.x * radius.x;
	r2.y = radius.y * radius.y;
	y = ft_max ((int)(center.y - radius.y), 0);
	while (y <= (int)(center.y + radius.y) && y < rt->win.frame_height)
	{
		x = ft_max ((int)(center.x - radius.y), 0);
		while (x <= (int)(center.x + radius.y) && x < rt->win.frame_width)
		{
			sqrd_len = ft_vec2f_sqrd_len(ft_vec2f(x - center.x, y - center.y));
			if (sqrd_len > r2.x && sqrd_len < r2.y)
				draw_pixel (rt, x, y, color);
			x += 1;
		}
		y += 1;
	}
}
