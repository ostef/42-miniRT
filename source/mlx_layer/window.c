/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 14:31:26 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/26 14:57:19 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_bool	create_window(t_window *win, t_cstr title, int width, int height)
{
	win->mlx = mlx_init();
	if (!win->mlx)
		return (FALSE);
	win->mlx_win = mlx_new_window(win->mlx, width, height, (t_str)title);
	if (!win->mlx_win)
		return (FALSE);
	win->mlx_frame_img = mlx_new_image(win->mlx, width, height);
	if (!win->mlx_frame_img)
	{
		mlx_destroy_window(win->mlx, win->mlx_win);
		return (FALSE);
	}
	win->pixels = mlx_get_data_addr(win->mlx_frame_img, &win->frame_bits_per_px,
						&win->frame_line_size, &win->frame_endianness);
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
	mlx_destroy_window(win->mlx, win->mlx_win);
	mlx_destroy_image(win->mlx, win->mlx_frame_img);
}

void	set_pixel(t_window *win, int x, int y, t_vec4f color)
{
	char	*dst;

	color.x = ft_clampf (color.x, 0, 1);
	color.y = ft_clampf (color.y, 0, 1);
	color.z = ft_clampf (color.z, 0, 1);
	dst = win->pixels + (y * win->frame_line_size + x * (win->frame_bits_per_px / 8));
	*(t_u32 *)dst = (((t_u8)(color.x * 255)) << 16) | (((t_u8)(color.y * 255)) << 8)
			| ((t_u8)(color.z * 255));
}

t_vec4f	get_pixel(t_window *win, int x, int y)
{
	t_u32	val;

	val = *(t_u32 *)(win->pixels + (y * win->frame_line_size + x * (win->frame_bits_per_px / 8)));
	return (ft_vec4f (
				((val >> 16) & 0xff) / 255.0f,
				((val >> 8) & 0xff) / 255.0f,
				(val & 0xff) / 255.0f,
				1));
}
