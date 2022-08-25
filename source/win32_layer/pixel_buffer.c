/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:47:08 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/24 13:47:08 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "win32_layer.h"

t_bool	create_framebuffer(t_window *win, t_int width, t_int height)
{
	ft_memset (&win->bmp_info, 0, sizeof (win->bmp_info));
	win->bmp_info.bmiHeader.biSize = sizeof (win->bmp_info.bmiHeader);
	win->bmp_info.bmiHeader.biWidth = width;
	win->bmp_info.bmiHeader.biHeight = height;
	win->bmp_info.bmiHeader.biPlanes = 1;
	win->bmp_info.bmiHeader.biBitCount = 32;
	win->bmp_info.bmiHeader.biCompression = BI_RGB;
	if (win->pixels)
		ft_free (win->pixels, ft_heap());
	win->pixels = ft_alloc (width * height * 4, ft_heap());
	if (!win->pixels)
		return (FALSE);
	win->frame_width = width;
	win->frame_height = height;
	return (TRUE);
}

t_bool	resize_framebuffer(t_window *win)
{
	RECT	rect;

	GetClientRect (win->hwnd, &rect);
	return (create_framebuffer (win, rect.right - rect.left, rect.bottom - rect.top));
}
