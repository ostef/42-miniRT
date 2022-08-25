/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 16:17:46 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/24 16:17:46 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_math.h"
#include "win32_layer.h"
#include <time.h>

static void	set_pixel(t_window *win, int x, int y, t_u32 color)
{
	((t_u32 *)win->pixels)[y * win->frame_width + x] = color;
}

static t_u32	rgb(t_u8 r, t_u8 g, t_u8 b)
{
	return (r << 16) | (g << 8) | b;
}

int	main(void)
{
	t_window	win;

	ft_memset (&win, 0, sizeof (win));
	if (!create_window (&win, "miniRT", 640, 480))
	{
		ft_fprintln (STDERR, "Could not create window.");
		return (1);
	}
	while (win.opened)
	{
		poll_window_events (&win);
		for (int y = 0; y < win.frame_height; y += 1)
		{
			for (int x = 0; x < win.frame_width; x += 1)
			{
				float r = x / (float)win.frame_width;
				float g = y / (float)win.frame_height;
				float b = (cosf ((float)GetTickCount64 () * 0.001) + 1) * 0.5;
				set_pixel (&win, x, y, rgb ((t_u8)(r * 255), (t_u8)(g * 255), (t_u8)(b * 255)));
			}
		}
		update_window (&win);
		Sleep (16);
	}
	destroy_window (&win);
	return (0);
}
