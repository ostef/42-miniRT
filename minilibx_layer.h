/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minilibx_layer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 13:49:37 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/22 17:11:24 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINILIBX_LAYER_H
# define MINILIBX_LAYER_H

# include "mlx.h"
# include "libft.h"
# include "ft_math.h"

typedef enum e_x11event
{
	KEY_PRESS = 2,
	KEY_RELEASE = 3,
	MOUSE_PRESS = 4,
	MOUSE_RELEASE = 5,
	MOUSE_MOVE = 6,
	ENTER_NOTIFY = 7,
	LEAVE_NOTIFY = 8,
	FOCUS_IN = 9,
	FOCUS_OUT = 10,
	KEYMAP_NOTIFY = 11,
	EXPOSE = 12,
	GRAPHICS_EXPOSE = 13,
	NO_EXPOSE = 14,
	VISIBILITY_NOTIFY = 15,
	CREATE_NOTIFY = 16,
	DESTROY_NOTIFY = 17,
	UNMAP_NOTIFY = 18,
	MAP_NOTIFY = 19,
	MAP_REQUEST = 20,
	REPARENT_NOTIFY = 21,
	CONFIGURE_NOTIFY = 22,
	CONFIGURE_REQUEST = 23,
	GRAVITY_NOTIFY = 24,
	RESIZE_REQUEST = 25,
	CIRCULATE_NOTIFY = 26,
	CIRCULATE_REQUEST = 27,
	PROPERTY_NOTIFY = 28,
	SELECTION_CLEAR = 29,
	SELECTION_REQUEST = 30,
	SELECTION_NOTIFY = 31,
	COLORMAP_NOTIFY = 32,
	CLIENT_MESSAGE = 33,
	MAPPING_NOTIFY = 34,
	GENERIC_EVENT = 35,
	LAST_EVENT = 36,
}	t_x11event;

typedef enum e_key
{
	KEY_A = 0,
	KEY_B = 11,
	KEY_C = 8,
	KEY_D = 2,
	KEY_E = 14,
	KEY_F = 3,
	KEY_G = 5,
	KEY_H = 4,
	KEY_I = 34,
	KEY_J = 38,
	KEY_K = 40,
	KEY_L = 37,
	KEY_M = 46,
	KEY_N = 45,
	KEY_O = 31,
	KEY_P = 35,
	KEY_Q = 12,
	KEY_R = 15,
	KEY_S = 1,
	KEY_T = 17,
	KEY_U = 32,
	KEY_V = 9,
	KEY_W = 13,
	KEY_X = 7,
	KEY_Y = 16,
	KEY_Z = 6,
	KEY_ZERO = 29,
	KEY_ONE = 18,
	KEY_TWO = 19,
	KEY_THREE = 20,
	KEY_FOUR = 21,
	KEY_FIVE = 23,
	KEY_SIX = 22,
	KEY_SEVEN = 26,
	KEY_EIGHT = 28,
	KEY_NINE = 25,
	KEY_BRACE_R = 30,
	KEY_BRACE_L = 33,
	KEY_TAB = 48,
	KEY_PLUS = 69,
	KEY_MINUS = 78,
	KEY_SEMI = 41,
	KEY_LEFT = 123,
	KEY_RIGHT = 124,
	KEY_DOWN = 125,
	KEY_UP = 126,
	KEY_ESCAPE = 53,
	KEY_LESS = 43,
	KEY_GREATER = 47,
	KEY_SPACE = 49,
	KEY_PAGE_UP = 116,
	KEY_PAGE_DOWN = 121,
	KEY_F2 = 120,
	KEY_CTRL = 256,
	KEY_SHIFT = 257
}	t_key;

typedef struct s_image
{
	void		*ptr;
	// void		*addr;
	t_int		bits_per_pixel;
	t_int		size_line;
	t_int		endian;
}	t_image;

// typedef struct s_window
// {
// 	t_s64		frame_width;
// 	t_s64		frame_height;
// 	void		*mlx;
// 	void		*window;
// 	t_image		img;
// 	void		*pixels;
// 	t_bool		opened;
// }	t_window;

typedef struct s_window
{
	void	*mlx;
	void	*window;
	t_s64	frame_width;
	t_s64	frame_height;
	t_image	image;
	void	*pixels;
	t_key	inputs[512];
	t_vec2f	mouse_coords;
	t_bool	opened;
}	t_window;

// typedef struct s_rt t_rt;

// t_bool	create_window(t_window *win, t_str title, int width, int height);
// void	destroy_window(t_window *win);
// // t_bool	allocate_framebuffer(t_window *win, t_int width, t_int height);
// void	poll_window_events(t_window *win);
// void	update_window(t_window *win);

// void	set_pixel(t_window *win, int x, int y, t_vec4f color);

// INPUTS

void	init_events(t_window *win);
// t_vec2f	get_mouse_pos(t_window *win);
// t_bool	is_key_down(t_window *win, int code);

#endif
