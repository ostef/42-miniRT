/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_events.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:21:48 by soumanso          #+#    #+#             */
/*   Updated: 2022/10/04 16:21:48 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	on_keydown(int keycode, void *win)
{
	t_window	*window;

	window = (t_window *)win;
	if (keycode == KEY_ESCAPE)
		on_destroy(keycode, win);
	window->inputs[keycode] = TRUE;
	return (0);
}

int	on_keyup(int keycode, void *win)
{
	t_window	*window;

	window = (t_window *)win;
	window->inputs[keycode] = FALSE;
	return (0);
}

int	on_mouse_press(int keycode, int x, int y, void *win)
{
	t_window	*window;

	window = (t_window *)win;
	window->inputs[keycode + 500] = TRUE;
	return (0);
	(void)x;
	(void)y;
}

int	on_mouse_release(int keycode, int x, int y, void *win)
{
	t_window	*window;

	window = (t_window *)win;
	window->inputs[keycode + 500] = FALSE;
	return (0);
	(void)x;
	(void)y;
}

int	on_mouse_move(int x, int y, void *win)
{
	t_window	*window;

	window = (t_window *)win;
	window->mouse_coords.x = x;
	window->mouse_coords.y = y;
	return (0);
}
