/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 17:07:40 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/25 17:07:40 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec2f	get_mouse_pos(t_window *win)
{
	POINT	pos;

	GetCursorPos (&pos);
	ScreenToClient (win->hwnd, &pos);
	return (ft_vec2f ((t_f32)pos.x, (t_f32)pos.y));
}

t_bool	is_key_down(t_window *win, t_key key)
{
	return (win->key_states[key] == KS_DOWN
		|| win->key_states[key] == KS_PRESSED);
}

t_bool	is_key_pressed(t_window *win, t_key key)
{
	return (win->key_states[key] == KS_PRESSED);
}

t_bool	is_key_released(t_window *win, t_key key)
{
	return (win->key_states[key] == KS_RELEASED);
}
