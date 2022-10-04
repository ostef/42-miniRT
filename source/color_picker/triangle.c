/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 13:35:26 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/29 13:35:26 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

#define SQRT3 1.73205080757

t_vec2f	sv_from_triangle_point(t_f32 x, t_f32 y)
{
	t_vec2f	res;

	res.x = ft_clampf((1 - 2 * y) / (SQRT3 * x - y + 2), 0.0001f, 1);
	res.y = ft_clampf((SQRT3 * x - y + 2) / 3, 0.0001f, 1);
	return (res);
}

t_vec2f	triangle_point_from_sv(t_f32 s, t_f32 v)
{
	t_vec2f	res;

	res.x = ((6 * s * v - 3 * s * s * v + s)
			/ (2 * s) - 2) / SQRT3;
	res.y = -(3 * v * s - 1) / 2;
	return (res);
}
