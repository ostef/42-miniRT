/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:30:30 by soumanso          #+#    #+#             */
/*   Updated: 2022/10/04 16:30:30 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	set_pixel(t_window *win, int x, int y, t_vec4f color)
{
	ft_assert(x >= 0 && x < win->frame_width && y >= 0 && y < win->frame_height,
		"set_pixel: invalid coordinates (%i, %i).", x, y);
	color.x = ft_clampf (color.x, 0, 1);
	color.y = ft_clampf (color.y, 0, 1);
	color.z = ft_clampf (color.z, 0, 1);
	((t_u32 *)win->pixels)[y * win->frame_width + x]
		= (((t_u8)(color.x * 255)) << 16) | (((t_u8)(color.y * 255)) << 8)
		| ((t_u8)(color.z * 255));
}

t_vec4f	get_pixel(t_window *win, int x, int y)
{
	t_u32	val;

	ft_assert(x >= 0 && x < win->frame_width && y >= 0 && y < win->frame_height,
		"get_pixel: invalid coordinates (%i, %i).", x, y);
	val = ((t_u32 *)win->pixels)[y * win->frame_width + x];
	return (ft_vec4f (
			((val >> 16) & 0xff) / 255.0f,
			((val >> 8) & 0xff) / 255.0f,
			(val & 0xff) / 255.0f,
			1));
}
