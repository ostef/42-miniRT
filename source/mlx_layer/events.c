/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:20:31 by soumanso          #+#    #+#             */
/*   Updated: 2022/10/04 16:20:31 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	on_destroy(int keycode, void *win)
{
	exit(0);
	(void)keycode;
	(void)win;
	return (0);
}

void	init_events(t_window *win)
{
	mlx_hook(win->mlx_win, MOUSE_PRESS, 0, on_mouse_press, win);
	mlx_hook(win->mlx_win, MOUSE_RELEASE, 0, on_mouse_release, win);
	mlx_hook(win->mlx_win, MOUSE_MOVE, 0, on_mouse_move, win);
	mlx_hook(win->mlx_win, KEY_PRESS, 0, on_keydown, win);
	mlx_hook(win->mlx_win, KEY_RELEASE, 0, on_keyup, win);
	mlx_hook(win->mlx_win, DESTROY_NOTIFY, 0, on_destroy, win);
}
