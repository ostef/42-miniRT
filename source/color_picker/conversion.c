/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 13:32:15 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/29 13:32:15 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static t_f32	fract_part(t_f32 x)
{
	return (x - floorf(x));
}

t_vec3f	hsv_to_rgb(t_vec3f hsv)
{
	t_vec3f	res;

	hsv.h /= 360.0;
	res.r = ft_absf(fract_part(hsv.h + 1) * 6 - 3) - 1;
	res.g = ft_absf(fract_part(hsv.h + 2 / 3.0) * 6 - 3) - 1;
	res.b = ft_absf(fract_part(hsv.h + 1 / 3.0) * 6 - 3) - 1;
	res.r = ft_clampf(res.r, 0, 1);
	res.g = ft_clampf(res.g, 0, 1);
	res.b = ft_clampf(res.b, 0, 1);
	res.r = ft_lerp(1, res.r, hsv.s);
	res.g = ft_lerp(1, res.g, hsv.s);
	res.b = ft_lerp(1, res.b, hsv.s);
	res = ft_vec3f_mulf (res, hsv.v);
	return (res);
}

t_vec3f	rgb_to_hsv(t_vec3f rgb)
{
	t_f32	min;
	t_f32	max;
	t_f32	delta;
	t_vec3f	res;

	min = ft_minf (rgb.r, ft_minf (rgb.g, rgb.b));
	max = ft_maxf (rgb.r, ft_maxf (rgb.g, rgb.b));
	if (max == 0)
		return (ft_vec3f (0, 0, 0));
	res.v = max;
	delta = max - min;
	res.s = delta / max;
	if (res.s == 0)
		return (ft_vec3f (0, 0, res.v));
	if (max == rgb.r)
		res.h = (43 / 255.0f) * (rgb.g - rgb.b) / delta;
	else if (max == rgb.g)
		res.h = (85 / 255.0f) + (43 / 255.0f) * (rgb.b - rgb.r) / delta;
	else
		res.h = (171 / 255.0f) + (43 / 255.0f) * (rgb.r - rgb.g) / delta;
	res.h *= 360;
	return (res);
}
