/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 16:03:34 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/22 18:27:00 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minilibx_layer.h"

int	destroy(int keycode, void *win)
{
	if (ft_get_heap_allocations () != 0)
		ft_fprintln (STDERR, "Found %i leaks.", ft_get_heap_allocations ());
	exit(0);
	(void)keycode;
	(void)win;
	return (0);
}

int	keydown(int keycode, void *win)
{
	t_window	*window;

	window = (t_window *)win;
	if (keycode == KEY_ESCAPE)
		destroy(keycode, win);
	window->inputs[keycode] = TRUE;
	return (0);
}

int	keyup(int keycode, void *win)
{
	t_window	*window;

	window = (t_window *)win;
	window->inputs[keycode] = FALSE;
	return (0);
}

int	mouse_press(int keycode, int x, int y, void *win)
{
	t_window	*window;

	window = (t_window *)win;
	window->inputs[keycode + 500] = TRUE;
	return (0);
	(void)x;
	(void)y;
}

int	mouse_release(int keycode, int x, int y, void *win)
{
	t_window	*window;

	window = (t_window *)win;
	window->inputs[keycode + 500] = TRUE;
	return (0);
	(void)x;
	(void)y;
}

int	mouse_move(int x, int y, void *win)
{
	t_window	*window;

	window = (t_window *)win;
	window->mouse_coords.x = x;
	window->mouse_coords.y = y;
	return (0);
}

t_bool	is_key_down(t_window *win, t_key key)
{
	return (win->inputs[code]);
}

t_vec2f	get_mouse_pos(t_window *win)
{
	return (ft_vec2f(win->mouse_coords.x, win->mouse_coords.y));
}


void	init_events(t_window *win)
{
	mlx_hook(win->window, MOUSE_PRESS, 0L, mouse_press, win);
	mlx_hook(win->window, MOUSE_RELEASE, 0L, mouse_release, win);
	mlx_hook(win->window, MOUSE_MOVE, 0L, mouse_move, win);
	mlx_hook(win->window, KEY_PRESS, 0L, keydown, win);
	mlx_hook(win->window, KEY_RELEASE, 0L, keyup, win);
	mlx_hook(win->window, DESTROY_NOTIFY, 0L, destroy, win);
}
