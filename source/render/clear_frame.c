/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_frame.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 14:55:59 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/24 14:55:59 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	clear_frame(t_rt *rt, t_vec4f color)
{
	int	x;
	int	y;

	y = 0;
	while (y < rt->win.frame_height)
	{
		x = 0;
		while (x < rt->win.frame_width)
		{
			set_pixel (&rt->win, x, y, color);
			x += 1;
		}
		y += 1;
	}
}
