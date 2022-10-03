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

static t_vec4f	calc_diffuse_for_light(t_rt *rt, t_hit_res hit, t_object obj)
{
	t_vec3f		pt_to_light;
	t_ray		ray;
	t_hit_res	sh;
	t_vec4f		diffuse;

	pt_to_light = ft_vec3f_direction (hit.point, obj.light.pos);
	ray.origin = ft_vec3f_add (hit.point, ft_vec3f_mulf (hit.normal, 0.001f));
	ray.dir = pt_to_light;
	ray.length = ft_vec3f_dist (hit.point, obj.light.pos);
	sh = raycast_first (rt, ray, FIL_SHAPES);
	if (sh.hit)
		diffuse.w = 0;
	else
		diffuse.w = ft_maxf (ft_vec3f_dot (hit.normal, pt_to_light), 0);
	diffuse = ft_vec4f_mulf (obj.color, obj.color.w * diffuse.w);
	return (diffuse);
}

static t_vec4f	calc_diffuse(t_rt *rt, t_hit_res hit)
{
	t_s64			i;
	t_vec4f			result;

	result = ft_vec4f (0, 0, 0, 0);
	i = 0;
	while (i < rt->obj_count)
	{
		if (rt->objs[i].type != LIGHT)
		{
			i += 1;
			continue ;
		}
		result = ft_vec4f_add (result,
				calc_diffuse_for_light (rt, hit, rt->objs[i]));
		i += 1;
	}
	return (result);
}

void	render_pixel(t_rt *rt, t_int px, t_int py)
{
	t_ray		ray;
	t_px_data	d;

	ray = ray_from_screen_point(rt, ft_vec2f(px + 0.5f, py + 0.5f), INVERT_Y);
	if (rt->is_editing)
		d.hit = raycast_closest (rt, ray, FIL_ALL);
	else
		d.hit = raycast_closest (rt, ray, FIL_SHAPES);
	if (d.hit.object)
	{
		if (d.hit.object->type == LIGHT)
			d.color = d.hit.object->color;
		else
		{
			d.color = d.hit.object->color;
			d.diffuse = calc_diffuse (rt, d.hit);
			d.ambient = ft_vec4f_mulf(rt->ambient_light, rt->ambient_light.w);
			d.light = ft_vec4f_add (d.diffuse, d.ambient);
			d.color.rgb = ft_vec3f_mulv (d.color.rgb, d.light.rgb);
		}
		if (rt->selected_object == d.hit.object)
			d.color = ft_vec4f_mulf (d.color, 2);
		set_pixel (&rt->win, px, py, d.color);
	}
}
