/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 14:06:21 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/20 14:08:13 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	poll_window_events(t_window *win)
{
	MSG	msg;
	int	key;

	while (PeekMessageA (&msg, win->hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage (&msg);
		DispatchMessageA (&msg);
	}
	ft_memcpy (win->prev_key_states, win->curr_key_states, sizeof (win->prev_key_states));
	key = 0;
	while (key < 256)
	{
		win->curr_key_states[key] = is_key_down (win, key);
		key += 1;
	}
}

static void	update_window(t_window *win)
{
	BITMAPINFO	bmi;

	ft_memset (&bmi, 0, sizeof (bmi));
	bmi.bmiHeader.biSize = sizeof (bmi.bmiHeader);
	bmi.bmiHeader.biWidth = win->frame_width;
	bmi.bmiHeader.biHeight = win->frame_height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	StretchDIBits (GetDC (win->hwnd),
		0, 0, win->frame_width, win->frame_height,
		0, 0, win->frame_width, win->frame_height,
		win->pixels, &bmi, DIB_RGB_COLORS, SRCCOPY);
}

void	main_loop(t_window *win, void (*f)(void *), void *data)
{
	while (win->opened)
	{
		poll_window_events (win);
		(*f)(data);
		update_window (win);
		Sleep (10);
	}
}
