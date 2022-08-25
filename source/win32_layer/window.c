/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:24:49 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/24 13:24:49 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "win32_layer.h"

static LRESULT	wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	t_window	*win;
	HDC			hdc;
	PAINTSTRUCT	paint;

	win = (t_window *)GetWindowLongPtrA (hwnd, GWLP_USERDATA);
	if (!win)
		return (DefWindowProcA (hwnd, msg, wparam, lparam));
	if (msg == WM_SIZE)
		resize_framebuffer (win);
	else if (msg == WM_CLOSE)
		win->opened = FALSE;
	return (DefWindowProcA (hwnd, msg, wparam, lparam));
}

static t_bool	register_wndclass(void)
{
	static t_bool		registered;
	static WNDCLASSA	wndclass;

	if (registered)
		return (TRUE);
	ft_memset (&wndclass, 0, sizeof (WNDCLASSA));
	wndclass.style = CS_OWNDC;
	wndclass.lpfnWndProc = wndproc;
	wndclass.hInstance = GetModuleHandleA (NULL);
	wndclass.lpszClassName = "WindowClass";
	wndclass.hCursor = LoadCursorA (NULL, IDC_ARROW);
	return (RegisterClassA (&wndclass) != 0);
}

t_bool	create_window(t_window *win, t_cstr title, int width, int height)
{
	RECT	rect;

	if (!register_wndclass())
		return (FALSE);
	win->hwnd = CreateWindowA("WindowClass", title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL,
		GetModuleHandleA (NULL), 0);
	if (!win->hwnd)
		return (FALSE);
	SetWindowLongPtrA (win->hwnd, GWLP_USERDATA, (LONG_PTR)win);
	GetClientRect (win->hwnd, &rect);
	if (!create_framebuffer (win, rect.right - rect.left, rect.bottom - rect.top))
		return (FALSE);
	win->opened = TRUE;
	return (TRUE);
}

void	destroy_window(t_window *win)
{
	DestroyWindow (win->hwnd);
}

void	poll_window_events(t_window *win)
{
	MSG	msg;
	int	key;

	while (PeekMessageA (&msg, win->hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage (&msg);
		DispatchMessageA (&msg);
	}
	ft_memcpy (g_keys.prev, g_keys.curr, sizeof (g_keys.prev));
	key = 0;
	while (key < 256)
	{
		g_keys.curr[key] = is_key_down (key);
		key += 1;
	}
}

void	update_window(t_window *win)
{
	StretchDIBits (GetDC (win->hwnd),
		0, 0, win->frame_width, win->frame_height,
		0, 0, win->frame_width, win->frame_height,
		win->pixels, &win->bmp_info, DIB_RGB_COLORS, SRCCOPY);
}
