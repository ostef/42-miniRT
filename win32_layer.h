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
	t_s64		frame_width;
	t_s64		frame_height;
	void		*pixels;
	t_bool		opened;
}	t_window;

typedef struct s_keyboard_state
{
	t_bool	prev[256];
	t_bool	curr[256];
}	t_keyboard_state;

t_bool	create_window(t_window *win, t_cstr title, int width, int height);
void	destroy_window(t_window *win);
t_bool	allocate_framebuffer(t_window *win, t_int width, t_int height);
void	poll_window_events(t_window *win);
void	update_window(t_window *win);

void	set_pixel(t_window *win, int x, int y, t_vec4f color);

typedef enum e_key
{
	UNKNOWN = 0,

	MOUSE_LEFT = VK_LBUTTON,
	MOUSE_MIDDLE = VK_MBUTTON,
	MOUSE_RIGHT = VK_RBUTTON,

	KEY_BACKSPACE = VK_BACK,
	KEY_TAB = VK_TAB,
	KEY_CLEAR = VK_CLEAR,
	KEY_RETURN = VK_RETURN,
	KEY_SHIFT = VK_SHIFT,
	KEY_CTRL = VK_CONTROL,
	KEY_ALT = VK_MENU,
	KEY_PAUSE = VK_PAUSE,
	KEY_CAPS_LOCK = VK_CAPITAL,
	KEY_ESCAPE = VK_ESCAPE,
	KEY_SPACE = VK_SPACE,
	KEY_PAGE_UP = VK_PRIOR,
	KEY_PAGE_DOWN = VK_NEXT,
	KEY_END = VK_END,
	KEY_HOME = VK_HOME,
	KEY_LEFT = VK_LEFT,
	KEY_UP = VK_UP,
	KEY_RIGHT = VK_RIGHT,
	KEY_DOWN = VK_DOWN,
	KEY_SELECT = VK_SELECT,
	KEY_PRINT = VK_PRINT,
	KEY_PRINT_SCREEN = VK_SNAPSHOT,
	KEY_INSERT = VK_INSERT,
	KEY_DELETE = VK_DELETE,
	KEY_HELP = VK_HELP,
	KEY_LEFT_SUPER = VK_LWIN,
	KEY_RIGHT_SUPER = VK_RWIN,
	KEY_NUMPAD0 = VK_NUMPAD0,
	KEY_NUMPAD1 = VK_NUMPAD1,
	KEY_NUMPAD2 = VK_NUMPAD2,
	KEY_NUMPAD3 = VK_NUMPAD3,
	KEY_NUMPAD4 = VK_NUMPAD4,
	KEY_NUMPAD5 = VK_NUMPAD5,
	KEY_NUMPAD6 = VK_NUMPAD6,
	KEY_NUMPAD7 = VK_NUMPAD7,
	KEY_NUMPAD8 = VK_NUMPAD8,
	KEY_NUMPAD9 = VK_NUMPAD9,
	KEY_NUMPAD_MULTIPLY = VK_MULTIPLY,
	KEY_NUMPAD_ADD = VK_ADD,
	KEY_NUMPAD_SEPARATOR = VK_SEPARATOR,
	KEY_NUMPAD_SUBTRACT = VK_SUBTRACT,
	KEY_NUMPAD_DECIMAL = VK_DECIMAL,
	KEY_NUMPAD_DIVIDE = VK_DIVIDE,
	KEY_F1 = VK_F1,
	KEY_F2 = VK_F2,
	KEY_F3 = VK_F3,
	KEY_F4 = VK_F4,
	KEY_F5 = VK_F5,
	KEY_F6 = VK_F6,
	KEY_F7 = VK_F7,
	KEY_F8 = VK_F8,
	KEY_F9 = VK_F9,
	KEY_F10 = VK_F10,
	KEY_F11 = VK_F11,
	KEY_F12 = VK_F12,
	KEY_NUM_LOCK = VK_NUMLOCK,
	KEY_SCROLL_LOCK = VK_SCROLL,
	KEY_LEFT_SHIFT = VK_LSHIFT,
	KEY_RIGHT_SHIFT = VK_RSHIFT,
	KEY_LEFT_CTRL = VK_LCONTROL,
	KEY_RIGHT_CTRL = VK_RCONTROL,
	KEY_LEFT_ALT = VK_LMENU,
	KEY_RIGHT_ALT = VK_RMENU,
}	t_key;

extern t_keyboard_state	g_keys;

t_vec2f	get_mouse_pos(t_window *win);
t_bool	was_key_down(t_key key);
t_bool	is_key_down(void *null, t_key key);
t_bool	is_key_pressed(t_key key);
t_bool	is_key_released(t_key key);

#endif
