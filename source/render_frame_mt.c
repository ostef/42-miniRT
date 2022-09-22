/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame_mt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 16:30:34 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/20 16:30:34 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

#ifndef FT_WIN32
# error "Multithreaded rendering is only implemented on Windows for now."
#endif

#ifndef THREADS_X
# define THREADS_X 4
#endif

#ifndef THREADS_Y
# define THREADS_Y 4
#endif

typedef struct s_thread_data
{
	t_rt	*rt;
	t_int	x;
	t_int	y;
}	t_thread_data;

static int	render_thread(void *param)
{
	t_thread_data	data;
	int				x;
	int				y;

	data = *(t_thread_data *)param;
	y = 0;
	while (y < data.rt->win.frame_height / THREADS_Y)
	{
		x = 0;
		while (x < data.rt->win.frame_width / THREADS_X)
		{
			render_pixel (data.rt, data.x + x, data.y + y);
			x += 1;
		}
		y += 1;
	}
	return (0);
}

void	render_frame(t_rt *rt)
{
	HANDLE			threads[THREADS_X * THREADS_Y];
	t_thread_data	d[THREADS_X * THREADS_Y];
	int				x;
	int				y;

	y = 0;
	while (y < THREADS_Y)
	{
		x = 0;
		while (x < THREADS_X)
		{
			d[y * THREADS_X + x].rt = rt;
			d[y * THREADS_X + x].x = x * rt->win.frame_width / THREADS_X;
			d[y * THREADS_X + x].y = y * rt->win.frame_height / THREADS_Y;
			threads[y * THREADS_X + x] = create_thread (
					&render_thread, &d[y * THREADS_X + x]);
			x += 1;
		}
		y += 1;
	}
	wait_for_threads(threads, THREADS_X * THREADS_Y);
	x = 0;
	while (x < THREADS_X * THREADS_Y)
		destroy_thread (threads[x++]);
}
