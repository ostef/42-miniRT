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

void	set_pixel(t_rt *rt, int x, int y, t_vec4f color)
{
	color.x = ft_clampf (color.x, 0, 1);
	color.y = ft_clampf (color.y, 0, 1);
	color.z = ft_clampf (color.z, 0, 1);
	((t_u32 *)rt->win.pixels)[y * rt->win.frame_width + x] =
			(((t_u8)(color.x * 255)) << 16) | (((t_u8)(color.y * 255)) << 8)
			| ((t_u8)(color.z * 255));
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
		allocate_framebuffer (win, rect.right - rect.left, rect.bottom - rect.top);
	}
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
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL,
		GetModuleHandleA (NULL), 0);
	if (!win->hwnd)
		return (FALSE);
	SetWindowLongPtrA (win->hwnd, GWLP_USERDATA, (LONG_PTR)win);
	GetClientRect (win->hwnd, &rect);
	if (!allocate_framebuffer (win, rect.right - rect.left, rect.bottom - rect.top))
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

t_bool	allocate_framebuffer(t_window *win, t_int width, t_int height)
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

void	update_window(t_window *win)
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
