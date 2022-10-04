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

#include "miniRT.h"

static t_bool	alloc_framebuffer(t_window *win, t_int width, t_int height)
{
	if (win->pixels)
		ft_free (win->pixels, ft_heap());
	win->pixels = ft_alloc (width * height * 4, ft_heap());
	if (!win->pixels)
		return (FALSE);
	win->frame_width = width;
	win->frame_height = height;
	return (TRUE);
}

static t_bool	handle_input_msg(HWND hwnd, UINT msg,
	WPARAM wparam, LPARAM lparam)
{
	t_window	*win;

	win = (t_window *)GetWindowLongPtrA (hwnd, GWLP_USERDATA);
	if (msg == WM_KEYDOWN)
	{
		if (((lparam >> 30) & 0x01) == 0)
			win->key_states[wparam] = KS_PRESSED;
	}
	else if (msg == WM_KEYUP)
		win->key_states[wparam] = KS_RELEASED;
	else if (msg == WM_LBUTTONDOWN)
		win->key_states[MOUSE_LEFT] = KS_PRESSED;
	else if (msg == WM_LBUTTONUP)
		win->key_states[MOUSE_LEFT] = KS_RELEASED;
	else if (msg == WM_MBUTTONDOWN)
		win->key_states[MOUSE_MIDDLE] = KS_PRESSED;
	else if (msg == WM_MBUTTONUP)
		win->key_states[MOUSE_MIDDLE] = KS_RELEASED;
	else if (msg == WM_RBUTTONDOWN)
		win->key_states[MOUSE_RIGHT] = KS_PRESSED;
	else if (msg == WM_RBUTTONUP)
		win->key_states[MOUSE_RIGHT] = KS_RELEASED;
	else
		return (FALSE);
	return (TRUE);
}

static LRESULT	wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	t_window	*win;
	RECT		rect;

	win = (t_window *)GetWindowLongPtrA (hwnd, GWLP_USERDATA);
	if (!win)
		return (DefWindowProcA (hwnd, msg, wparam, lparam));
	if (msg == WM_SIZE)
	{
		GetClientRect (win->hwnd, &rect);
		alloc_framebuffer (win, rect.right - rect.left, rect.bottom - rect.top);
	}
	else if (msg == WM_CLOSE)
		win->opened = FALSE;
	else if (!handle_input_msg (hwnd, msg, wparam, lparam))
		return (DefWindowProcA (hwnd, msg, wparam, lparam));
	return (0);
}

t_bool	create_window(t_window *win, t_cstr title, int width, int height)
{
	static t_bool		registered;
	static WNDCLASSA	wndclass;
	RECT				r;

	if (!registered)
	{
		wndclass.style = CS_OWNDC;
		wndclass.lpfnWndProc = wndproc;
		wndclass.hInstance = GetModuleHandleA (NULL);
		wndclass.lpszClassName = "WindowClass";
		wndclass.hCursor = LoadCursorA (NULL, IDC_ARROW);
		if (!RegisterClassA (&wndclass))
			return (FALSE);
		registered = TRUE;
	}
	ft_memset (win, 0, sizeof (t_window));
	win->hwnd = CreateWindowA("WindowClass", title,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
			width, height, NULL, NULL, GetModuleHandleA (NULL), 0);
	SetWindowLongPtrA (win->hwnd, GWLP_USERDATA, (LONG_PTR)win);
	GetClientRect (win->hwnd, &r);
	if (!alloc_framebuffer(win, r.right - r.left, r.bottom - r.top))
		return (FALSE);
	win->opened = TRUE;
	return (TRUE);
}

void	destroy_window(t_window *win)
{
	DestroyWindow (win->hwnd);
}
