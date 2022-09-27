/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_rect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 12:32:36 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/27 12:32:36 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	draw_rect(t_rt *rt, t_vec2f min, t_vec2f max, t_vec4f color)
{
	int	x;
	int	y;

	y = ft_max ((int)min.y, 0);
	while (y <= (int)max.y && y < rt->win.frame_height)
	{
		x = ft_max ((int)min.x, 0);
		while (x <= (int)max.x && x < rt->win.frame_width)
		{
			draw_pixel (rt, x, y, color);
			x += 1;
		}
		y += 1;
	}
}
