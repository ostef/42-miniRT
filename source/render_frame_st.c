/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame_st.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 16:30:34 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/20 16:30:34 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	render_frame(t_rt *rt)
{
	int	x;
	int	y;
	
	y = 0;
	while (y < rt->win.frame_height)
	{
		x = 0;
		while (x < rt->win.frame_width)
		{
			render_pixel (rt, x, y);
			x += 1;
		}
		y += 1;
	}
}
