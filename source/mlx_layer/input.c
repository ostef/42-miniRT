/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 16:03:34 by ljourand          #+#    #+#             */
/*   Updated: 2022/10/04 16:21:30 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec2f	get_mouse_pos(t_window *win)
{
	return (win->mouse_coords);
}

t_bool	is_key_down(t_window *win, t_key key)
{
	return (win->curr_inputs[key]);
}

t_bool	is_key_pressed(t_window *win, t_key key)
{
	return (!win->prev_inputs[key] && win->curr_inputs[key]);
}

t_bool	is_key_released(t_window *win, t_key key)
{
	return (win->prev_inputs[key] && !win->curr_inputs[key]);
}
