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

# define THREAD_X_COUNT (4)
# define THREAD_Y_COUNT (4)
# define THREAD_COUNT (THREAD_X_COUNT * THREAD_Y_COUNT)

typedef struct s_thread_data
{
	t_rt	*rt;
	t_int	x0;
	t_int	y0;
	t_int	x1;
	t_int	y1;
}	t_thread_data;

static DWORD	render_thread(void *param)
{
	t_thread_data	data;
	int				x;
	int				y;
	
	data = *(t_thread_data *)param;
	y = data.y0;
	while (y < data.y1)
	{
		x = data.x0;
		while (x < data.x1)
		{
			render_pixel (data.rt, x, y);
			x += 1;
		}
		y += 1;
	}
	return (0);
}

void	render_frame(t_rt *rt)
{
	HANDLE	threads[THREAD_COUNT] = {0};
	t_thread_data	thread_data[THREAD_COUNT] = {0};

	for (int y = 0; y < THREAD_Y_COUNT; y += 1)
	{
		for (int x = 0; x < THREAD_X_COUNT; x += 1)
		{
			int i = y * THREAD_X_COUNT + x;
			thread_data[i].rt = rt;
			thread_data[i].x0 = x * rt->win.frame_width / THREAD_X_COUNT;
			thread_data[i].x1 = (x + 1) * rt->win.frame_width / THREAD_X_COUNT;
			thread_data[i].y0 = y * rt->win.frame_height / THREAD_Y_COUNT;
			thread_data[i].y1 = (y + 1) * rt->win.frame_height / THREAD_Y_COUNT;
			threads[i] = CreateThread (NULL, 0, render_thread, &thread_data[i], 0, NULL);
		}
	}

	WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE);

	for (int i = 0; i < THREAD_COUNT; i += 1)
	{
		CloseHandle (threads[i]);
	}
}
