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

t_vec2f	sv_from_triangle_point(t_vec2f p)
{
	t_vec2f	res;

	res.x = ft_clampf((1 - 2 * p.y) / (SQRT3 * p.x - p.y + 2), 0.0001f, 1);
	res.y = ft_clampf((SQRT3 * p.x - p.y + 2) / 3, 0.0001f, 1);
	return (res);
}

t_vec2f	triangle_point_from_sv(t_vec2f sv)
{
	t_vec2f	res;

	res.x = ((6 * sv.x * sv.y - 3 * sv.x * sv.x * sv.y + sv.x)
			/ (2 * sv.x) - 2) / SQRT3;
	res.y = -(3 * sv.y * sv.x - 1) / 2;
	return (res);
}
