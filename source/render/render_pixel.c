/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pixel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 14:47:20 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/24 14:47:20 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

typedef struct s_px_data
{
	t_hit_res	hit;
	t_hit_res	shadow_hit;
	t_vec3f		point_to_light;
	t_f32		diffuse_intensity;
	t_vec4f		diffuse;
	t_vec4f		ambient;
	t_vec4f		light;
	t_vec4f		color;
}	t_px_data;

#ifdef FT_WIN32
# define INVERT_Y 1
#else
# define INVERT_Y 0
#endif

static t_vec4f	calc_diffuse(t_rt *rt, t_hit_res hit)
{
	t_s64		i;
	t_object	obj;
	t_ray		shadow_ray;
	t_hit_res	shadow_hit;
	t_vec4f		diffuse;
	t_vec4f		result;
	t_vec3f		point_to_light;

	result = ft_vec4f (0, 0, 0, 0);
	i = 0;
	while (i < rt->obj_count)
	{
		if (rt->objs[i].type != LIGHT)
		{
			i += 1;
			continue ;
		}
		obj = rt->objs[i];
		point_to_light = ft_vec3f_normalized (ft_vec3f_sub (obj.light.pos, hit.point));
		shadow_ray.origin = hit.point;
		shadow_ray.dir = point_to_light;
		shadow_hit = raycast_closest_except (rt, shadow_ray, hit.object);
		if (shadow_hit.hit && shadow_hit.dist * shadow_hit.dist < ft_vec3f_sqrd_dist (obj.light.pos, shadow_ray.origin))
			diffuse.w = 0;
		else
			diffuse.w = ft_maxf (ft_vec3f_dot (hit.normal, point_to_light), 0);
		diffuse = ft_vec4f_mulf (obj.color, obj.color.w * diffuse.w);
		result = ft_vec4f_add (result, diffuse);
		i += 1;
	}
	return (result);
}

void	render_pixel(t_rt *rt, t_int px_x, t_int px_y)
{
	t_ray		ray;
	t_px_data	dat;

	ray = ray_from_screen_point (rt, ft_vec2f (px_x + 0.5f, px_y + 0.5f), INVERT_Y);
	dat.hit = raycast_closest (rt, ray);
	if (dat.hit.object)
	{
		if (rt->selected_object == dat.hit.object)
		{
			set_pixel (&rt->win, px_x, px_y, ft_vec4f (1, 1, 1, 1));
		}
		else
		{
			dat.color = dat.hit.object->color;
			dat.diffuse = calc_diffuse (rt, dat.hit);
			dat.ambient = ft_vec4f_mulf (rt->ambient_light, rt->ambient_light.w);
			dat.light = ft_vec4f_add (dat.diffuse, dat.ambient);
			dat.color.r *= dat.light.r;
			dat.color.g *= dat.light.g;
			dat.color.b *= dat.light.b;
			set_pixel (&rt->win, px_x, px_y, dat.color);
		}
	}
}
