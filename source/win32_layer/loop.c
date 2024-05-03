/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 14:06:21 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/23 16:16:54 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	poll_window_events(t_window *win)
{
	MSG	msg;
	int	key;

	key = 0;
	while (key < 256)
	{
		if (win->key_states[key] == KS_PRESSED)
			win->key_states[key] = KS_DOWN;
		else if (win->key_states[key] == KS_RELEASED)
			win->key_states[key] = KS_UP;
		key += 1;
	}
	while (PeekMessageA (&msg, win->hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage (&msg);
		DispatchMessageA (&msg);
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

void	main_loop(t_window *win, int (*f)(void *), void *data)
{
	while (win->opened)
	{
		poll_window_events (win);
		(*f)(data);
		update_window (win);
		Sleep (10);
	}
}
