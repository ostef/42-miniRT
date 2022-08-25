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

#include "miniRT.h"

static inline void	set_pixel(t_window *win, int x, int y, t_u32 color)
{
	((t_u32 *)win->pixels)[y * win->frame_width + x] = color;
}

static inline void	clear_frame(t_window *win, t_u32 color)
{
	for (int i = 0; i < win->frame_width * win->frame_height; i += 1)
		((t_u32 *)win->pixels)[i] = color;
}

#define	rgb(r, g, b) (((r) << 16) | ((g) << 8) | (b))

int	main(void)
{
	t_window	win;

	ft_memset (&win, 0, sizeof (win));
	if (!create_window (&win, "miniRT", 640, 480))
	{
		ft_fprintln (STDERR, "Could not create window.");
		return (1);
	}
	t_vec2f	offset = ft_vec2f(0,0);
	while (win.opened)
	{
		poll_window_events (&win);
		clear_frame (&win, 0);

		offset = get_mouse_pos (&win);
		for (int y = 0; y < win.frame_height; y += 1)
		{
			for (int x = 0; x < win.frame_width; x += 1)
			{
				set_pixel (&win, x, y, rgb ((t_u8)(x + offset.x * 255), (t_u8)(y - offset.y * 255), 0));
			}
		}
		update_window (&win);
		Sleep (10);
	}
	destroy_window (&win);
	return (0);
}
