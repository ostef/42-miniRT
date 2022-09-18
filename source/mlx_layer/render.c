/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 14:09:54 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/16 14:29:23 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	render(int (*f)(t_rt *), t_rt *rt)
{
	mlx_loop_hook(rt->win.mlx, *f, rt);
	mlx_loop(rt->win.mlx);
}

