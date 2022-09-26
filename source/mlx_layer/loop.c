/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 14:09:54 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/26 14:42:28 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

typedef struct s_mlx_loop
{
	t_window	*win;
	void		*data;
	int			(*f)(void *);
}	t_mlx_loop;

static int	loop_wrapper(void *data)
{
	t_mlx_loop	loop;

	loop = *(t_mlx_loop *)data;
	ft_memcpy (loop.win->prev_inputs, loop.win->inputs,
		sizeof (loop.win->prev_inputs));
	loop.f (loop.data);
	update_window (loop.win);
}

void	main_loop(t_window *win, int (*f)(void *), void *data)
{
	t_mlx_loop	loop;

	loop.win = win;
	loop.data = data;
	loop.f = f;
	mlx_loop_hook(win->mlx, &loop_wrapper, &loop);
	mlx_loop(win->mlx);
}

