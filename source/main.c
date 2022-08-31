/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 16:17:46 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/24 16:17:46 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	set_pixel(t_rt *rt, int x, int y, t_vec4f color)
{
	color.r = ft_clampf (color.r, 0, 1);
	color.g = ft_clampf (color.g, 0, 1);
	color.b = ft_clampf (color.b, 0, 1);
	((t_u32 *)rt->win.pixels)[y * rt->win.frame_width + x] =
			(((t_u8)(color.r * 255)) << 16) | (((t_u8)(color.g * 255)) << 8)
			| ((t_u8)(color.b * 255));
}

void	render_pixel(t_rt *rt, t_int px_x, t_int px_y)
{
	t_ray			ray;
	t_hit_result	hit;
	t_f32			diffuse_intensity;

	ray.origin = ft_mat4f_transform_point(rt->camera.transform, rt->camera.position);
	ray.dir = ft_vec3f (
		(2 * (px_x + 0.5f) / rt->camera.width - 1) * rt->camera.aspect_ratio * rt->camera.scale,
		(1 - 2 * (px_y + 0.5f) / rt->camera.height) * rt->camera.scale, 1);
	ray.dir = ft_mat4f_transform_vector(rt->camera.transform, ft_vec3f_normalized (ray.dir));
	hit = raycast_closest (rt, ray);
	if (hit.object)
	{
		t_vec3f	point_to_light = ft_vec3f_normalized (ft_vec3f_sub (rt->light_position, hit.point));
		diffuse_intensity = ft_vec3f_dot (hit.normal, point_to_light);
		if (diffuse_intensity < 0)
			diffuse_intensity = 0;

		ray.origin = hit.point;
		ray.dir = point_to_light;
		if (raycast_first_except (rt, ray, hit.object).hit)
			diffuse_intensity = 0;
		t_vec4f	diffuse = ft_vec4f_mulf (rt->light_color, rt->light_color.w * diffuse_intensity);
		t_vec4f	ambient = ft_vec4f_mulf (rt->ambient_light, rt->ambient_light.w);
		t_vec4f	light = ft_vec4f_add (diffuse, ambient);
		t_vec4f	color = hit.object->color;
		color.r *= light.r;
		color.g *= light.g;
		color.b *= light.b;
		set_pixel (rt, px_x, px_y, color);
	}
}

int	main(void)
{
	t_rt	rt;

	ft_memset (&rt, 0, sizeof (rt));
	if (!create_window (&rt.win, "miniRT", 640, 480))
	{
		ft_fprintln (STDERR, "Could not create window.");
		return (1);
	}

	rt.light_position = ft_vec3f (10, 10, 10);
	rt.light_color = ft_vec4f (0.7f, 0.4f, 0.3f, 0.7f);
	rt.ambient_light = ft_vec4f (0.7f, 0.4f, 0.3f, 0.2f);
	rt.camera.direction = ft_vec3f(0, 0, 1);
	rt.camera.fov_in_degrees = 60.0f;
	add_sphere (&rt, ft_vec3f (0, 1, 20), 10.0)->color = ft_vec4f(1, 0, 0, 0);
	add_sphere (&rt, ft_vec3f (1, -8, 15), 9.0)->color = ft_vec4f(0, 1, 0, 0);
	add_sphere (&rt, ft_vec3f (-3.5, -1.4, 60), 20.0)->color = ft_vec4f(0, 0, 1, 0);

	while (rt.win.opened)
	{
		t_u64	start = GetTickCount64 ();

		poll_window_events (&rt.win);

		for (int y = 0; y < rt.win.frame_height; y += 1)
			for (int x = 0; x < rt.win.frame_width; x += 1)
				set_pixel (&rt, x, y, ft_vec4f_mulf (rt.ambient_light, rt.ambient_light.w));

		rt.camera.width = rt.win.frame_width;
		rt.camera.height = rt.win.frame_height;
		rt.camera.scale = tanf (rt.camera.fov_in_degrees * 0.5f * PI / 180.0f);
		rt.camera.position.x += is_key_down ('D') - is_key_down ('A');
		rt.camera.position.y += is_key_down ('E') - is_key_down ('Q');
		rt.camera.position.z += is_key_down ('W') - is_key_down ('S');
		rt.camera.aspect_ratio = rt.camera.width / rt.camera.height;
		rt.camera.transform = ft_mat4f_look_at(rt.camera.position, rt.camera.direction);

		for (int y = 0; y < rt.win.frame_height; y += 1)
			for (int x = 0; x < rt.win.frame_width; x += 1)
				render_pixel (&rt, x, y);

		update_window (&rt.win);

		ft_println ("%u", GetTickCount64 () - start);
		Sleep (10);
	}
	destroy_window (&rt.win);
	return (0);
}
