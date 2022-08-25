/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   win32_layer.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 11:19:06 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/25 11:19:06 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef WIN32_LAYER_H
# define WIN32_LAYER_H

# include "libft.h"
# include "ft_math.h"
# include <Windows.h>

typedef struct s_window
{
	HWND		hwnd;
	BITMAPINFO	bmp_info;
	void		*pixels;
	t_s64		frame_width;
	t_s64		frame_height;
	t_bool		opened;
}	t_window;

t_bool	create_window(t_window *win, t_cstr title, int width, int height);
t_bool	create_framebuffer(t_window *win, t_int width, t_int height);
void	destroy_window(t_window *win);
void	poll_window_events(t_window *win);
void	update_window(t_window *win);

typedef enum e_key
{
	UNKNOWN = 0,

	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT,

	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CLEAR,
	KEY_RETURN,
	KEY_SHIFT,
	KEY_CTRL,
	KEY_ALT,
	KEY_PAUSE,
	KEY_CAPS_LOCK,
	KEY_ESCAPE,
	KEY_SPACE,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,
	KEY_END,
	KEY_HOME,
	KEY_LEFT,
	KEY_UP,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_SELECT,
	KEY_PRINT,
	KEY_PRINT_SCREEN,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HELP,
	KEY_LEFT_SUPER,
	KEY_RIGHT_SUPER,
	KEY_APPS,
	KEY_SLEEP,
	KEY_NUMPAD0,
	KEY_NUMPAD1,
	KEY_NUMPAD2,
	KEY_NUMPAD3,
	KEY_NUMPAD4,
	KEY_NUMPAD5,
	KEY_NUMPAD6,
	KEY_NUMPAD7,
	KEY_NUMPAD8,
	KEY_NUMPAD9,
	KEY_NUMPAD_MULTIPLY,
	KEY_NUMPAD_ADD,
	KEY_NUMPAD_SEPARATOR,
	KEY_NUMPAD_SUBTRACT,
	KEY_NUMPAD_DECIMAL,
	KEY_NUMPAD_DIVIDE,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_NUM_LOCK,
	KEY_SCROLL_LOCK,
	KEY_LEFT_SHIFT,
	KEY_RIGHT_SHIFT,
	KEY_LEFT_CTRL,
	KEY_RIGHT_CTRL,
	KEY_LEFT_ALT,
	KEY_RIGHT_ALT,
}	t_key;

int		key_to_vk(t_key key);
t_key	vk_to_key(int vk);

t_bool	is_key_down(t_key key);
t_bool	is_key_up(t_key key);
t_bool	is_key_pressed(t_key key);
t_bool	is_key_released(t_key key);

t_vec2f	get_mouse_pos(void);
t_bool	is_mouse_button_down(t_key btn);
t_bool	is_mouse_button_up(t_key btn);
t_bool	is_mouse_button_pressed(t_key btn);
t_bool	is_mouse_button_released(t_key btn);

#endif
