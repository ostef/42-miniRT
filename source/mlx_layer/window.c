/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 14:31:26 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/14 17:47:17 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minilibx_layer.h"

t_bool	create_window(t_window *win, t_str title, int width, int height)
{
	win->mlx = mlx_init();
	if (!win->mlx)
		return (FALSE);
	win->window = mlx_new_window(win->mlx, width, height, title);
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
	*dst = (((t_u8)(color.x * 255)) << 16) | (((t_u8)(color.y * 255)) << 8)
			| ((t_u8)(color.z * 255));
}

void	poll_window_events(t_window *win)
{
	(void) win;
}
