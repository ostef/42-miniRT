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

void	render_pixel(t_rt *rt, t_int px_x, t_int px_y)
{
	t_ray		ray;
	t_px_data	dat;

	ray = ray_from_screen_point (rt, ft_vec2f (px_x + 0.5f, px_y + 0.5f), TRUE);
	dat.hit = raycast_closest (rt, ray);
	if (dat.hit.object)
	{
		if (rt->selected_object == dat.hit.object)
		{
			set_pixel (&rt->win, px_x, px_y, ft_vec4f (1, 1, 1, 1));
		}
		else
		{
			dat.point_to_light = ft_vec3f_normalized (ft_vec3f_sub (rt->light_position, dat.hit.point));
			ray.origin = dat.hit.point;
			ray.dir = dat.point_to_light;
			dat.shadow_hit = raycast_closest_except (rt, ray, dat.hit.object);
			if (dat.shadow_hit.hit && dat.shadow_hit.dist < ft_vec3f_dist (rt->light_position, ray.origin))
				dat.diffuse_intensity = 0;
			else
				dat.diffuse_intensity = ft_maxf (ft_vec3f_dot (dat.hit.normal, dat.point_to_light), 0);
			dat.color = dat.hit.object->color;
			dat.diffuse = ft_vec4f_mulf (rt->light_color, rt->light_color.w * dat.diffuse_intensity);
			dat.ambient = ft_vec4f_mulf (rt->ambient_light, rt->ambient_light.w);
			dat.light = ft_vec4f_add (dat.diffuse, dat.ambient);
			dat.color.r *= dat.light.r;
			dat.color.g *= dat.light.g;
			dat.color.b *= dat.light.b;
			set_pixel (&rt->win, px_x, px_y, dat.color);
		}
	}
}
