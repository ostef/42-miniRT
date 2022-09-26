/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 14:31:26 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/26 13:39:09 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_bool	create_window(t_window *win, t_cstr title, int width, int height)
{
	win->mlx = mlx_init();
	if (!win->mlx)
		return (FALSE);
	win->window = mlx_new_window(win->mlx, width, height, (t_str)title);
	if (!win->window)
		return (FALSE);
	win->image.ptr = mlx_new_image(win->mlx, width, height);
	if (!win->image.ptr)
	{
		mlx_destroy_window(win->mlx, win->window);
		return (FALSE);
	}
	win->pixels = mlx_get_data_addr(win->image.ptr, &win->image.bits_per_pixel,
						&win->image.size_line, &win->image.endian);
	if (!win->pixels)
	{
		destroy_window(win);
		return (FALSE);
	}
	win->frame_width = width;
	win->frame_height = height;
	win->opened = TRUE;
	init_events(win);
	return (TRUE);
}

void	destroy_window(t_window *win)
{
	mlx_destroy_window(win->mlx, win->window);
	mlx_destroy_image(win->mlx, win->image.ptr);
}

void	update_window(t_window *win)
{
	mlx_put_image_to_window(win->mlx, win->window, win->image.ptr, 0, 0);
}

void	set_pixel(t_window *win, int x, int y, t_vec4f color)
{
	char	*dst;

	color.x = ft_clampf (color.x, 0, 1);
	color.y = ft_clampf (color.y, 0, 1);
	color.z = ft_clampf (color.z, 0, 1);
	dst = win->pixels + (y * win->image.size_line + x * (win->image.bits_per_pixel / 8));
	*(t_u32 *)dst = (((t_u8)(color.x * 255)) << 16) | (((t_u8)(color.y * 255)) << 8)
			| ((t_u8)(color.z * 255));
	// *dst = ((255 - x.a) << 24) | (x.r << 16) | (x.g << 8) | (x.b)
}
