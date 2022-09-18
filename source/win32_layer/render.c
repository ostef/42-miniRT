/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 14:06:21 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/16 14:31:35 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	render(void (*f)(t_rt *), t_rt *rt)
{
	while (rt->win.opened)
	{
		poll_window_events (&rt->win);
		(*f)(rt);
		Sleep (10);
	}
}
