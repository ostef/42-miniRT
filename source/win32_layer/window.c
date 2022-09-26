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
	else if (msg == WM_KEYDOWN)
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
	return (DefWindowProcA (hwnd, msg, wparam, lparam));
}

t_bool	create_window(t_window *win, t_cstr title, int width, int height)
{
	static t_bool		registered;
	static WNDCLASSA	wndclass;
	RECT				rect;

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
	win->hwnd = CreateWindowA("WindowClass", title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL,
		GetModuleHandleA (NULL), 0);
	if (!win->hwnd)
		return (FALSE);
	SetWindowLongPtrA (win->hwnd, GWLP_USERDATA, (LONG_PTR)win);
	GetClientRect (win->hwnd, &rect);
	if (!alloc_framebuffer (win, rect.right - rect.left, rect.bottom - rect.top))
		return (FALSE);
	win->opened = TRUE;
	return (TRUE);
}

void	destroy_window(t_window *win)
{
	DestroyWindow (win->hwnd);
}

void	set_pixel(t_window *win, int x, int y, t_vec4f color)
{
	color.x = ft_clampf (color.x, 0, 1);
	color.y = ft_clampf (color.y, 0, 1);
	color.z = ft_clampf (color.z, 0, 1);
	((t_u32 *)win->pixels)[y * win->frame_width + x] =
			(((t_u8)(color.x * 255)) << 16) | (((t_u8)(color.y * 255)) << 8)
			| ((t_u8)(color.z * 255));
}

t_vec4f	get_pixel(t_window *win, int x, int y)
{
	t_u32	val;

	val = ((t_u32 *)win->pixels)[y * win->frame_width + x];
	return (ft_vec4f (
				((val >> 16) & 0xff) / 255.0f,
				((val >> 8) & 0xff) / 255.0f,
				(val & 0xff) / 255.0f,
				1));
}
