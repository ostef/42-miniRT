/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 14:09:54 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/22 18:47:35 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	main_loop(t_window *win, int (*f)(void *), void *data)
{
	t_rt	*rt;

	rt = (t_rt *)data;
	mlx_loop_hook(win->mlx, *f, rt);
	mlx_loop(win->mlx);
}

