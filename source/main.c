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
	color.x = ft_clampf (color.x, 0, 1);
	color.y = ft_clampf (color.y, 0, 1);
	color.z = ft_clampf (color.z, 0, 1);
	((t_u32 *)rt->win.pixels)[y * rt->win.frame_width + x] =
			(((t_u8)(color.x * 255)) << 16) | (((t_u8)(color.y * 255)) << 8)
			| ((t_u8)(color.z * 255));
}

void	render_pixel(t_rt *rt, t_int px_x, t_int px_y)
{
	t_ray			ray;
	t_hit_result	hit;
	t_f32			diffuse_intensity;

	ray.origin = rt->camera.position;
	ray.dir = ft_vec3f (
		(2 * (px_x + 0.5f) / rt->camera.width - 1) * rt->camera.aspect_ratio * rt->camera.scale,
		(1 - 2 * (px_y + 0.5f) / rt->camera.height) * rt->camera.scale, 1);
	ray.dir = ft_mat4f_transform_vector(rt->camera.transform, ft_vec3f_normalized (ray.dir));
	hit = raycast_closest (rt, ray);
	if (hit.object)
	{
		t_vec3f	point_to_light = ft_vec3f_normalized (ft_vec3f_sub (rt->light_position, hit.point));
		ray.origin = hit.point;
		ray.dir = point_to_light;
		if (raycast_first_except (rt, ray, hit.object).hit)
			diffuse_intensity = 0;
		else
			diffuse_intensity = ft_maxf (ft_vec3f_dot (hit.normal, point_to_light), 0);

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

int	main(int ac, char **av)
{
	t_rt	rt;

	ft_memset (&rt, 0, sizeof (rt));
	ft_fprintln (STDERR, "file: %s", av[1]);
	rt = parsing(av[1]);
	if (!create_window (&rt.win, "miniRT", 640, 480))
	{
		ft_fprintln (STDERR, "Could not create window.");
		return (1);
	}
	 

	float scale = tanf (60.0f * 0.5f * PI / 180.0f);
	// rt.light_position = ft_vec3f (10, 10, 10);
	// rt.light_color.rgb = ft_vec3f (1, 1, 1);
	// rt.light_color.w = 0.7f;
	// rt.ambient_light.rgb = ft_vec3f (0.7, 0.6, 0.4);
	// rt.ambient_light.w = 0.2f;
	// rt.camera.fov_in_degrees = 60.0f;
	// rt.camera.transform = ft_mat4f_identity ();

	// add_sphere (&rt, ft_vec3f (0, 1, 20), 5.0)->color = ft_vec4f(1, 0, 0, 0);
	// add_sphere (&rt, ft_vec3f (1, -8, 15), 2.0)->color = ft_vec4f(0, 1, 0, 0);
	// add_sphere (&rt, ft_vec3f (-10, 20, 0), 7.0)->color = ft_vec4f(0, 0, 1, 0);
	// add_plane (&rt, ft_vec3f (0, 70, 0), ft_vec3f (0, 1, 0))->color = ft_vec4f(1, 0.2, 0.4, 0);

	// C 0,0,0 0,0,0 60

	// sp 0,1,20 5 255,0,0
	// sp 1,-8,15 2 0,255,0
	// sp -10,20,0 7 0,0,255

	// A 0.2 179,153,102

	// pl 0,70,0 0,1,0 255,51,102

	// L 10,10,10 0.7 255,255,255

	t_object	*planet = add_sphere (&rt, ft_vec3f (0, 0, 100), 60.0);
	planet->color = ft_vec4f(1, 0, 1, 0);


	t_object	*obj = add_object (&rt);
	obj->shape = CYLINDER;
	obj->cylinder.bottom = ft_vec3f (0, 0, 0);
	obj->cylinder.top = ft_vec3f (0, 5, 0);
	obj->cylinder.radius = 3;
	obj->color = ft_vec4f (1, 0, 0, 0);

	while (rt.win.opened)
	{
		t_u64	start = GetTickCount64 ();
		t_f32	seconds = ((t_f32)start) / 1000.0f;

		poll_window_events (&rt.win);

		for (int y = 0; y < rt.win.frame_height; y += 1)
			for (int x = 0; x < rt.win.frame_width; x += 1)
				set_pixel (&rt, x, y, ft_vec4f_mulf (rt.ambient_light, rt.ambient_light.w));

		rt.camera.width = rt.win.frame_width;
		rt.camera.height = rt.win.frame_height;
		rt.camera.scale = tanf (rt.camera.fov_in_degrees * 0.5f * PI / 180.0f);
		t_f32	speed = 1;
		if (is_key_down (KEY_SHIFT))
			speed = 10;

		t_vec3f	right = ft_mat4f_right_vector (rt.camera.transform);
		t_vec3f	up = ft_mat4f_up_vector (rt.camera.transform);
		t_vec3f	forward = ft_mat4f_forward_vector (rt.camera.transform);

		rt.camera.yaw += (is_key_down (KEY_RIGHT) - is_key_down (KEY_LEFT)) * 2;
		rt.camera.pitch += (is_key_down (KEY_UP) - is_key_down (KEY_DOWN)) * 2;
		rt.camera.pitch = ft_clampf (rt.camera.pitch, -80, 80);

		rt.camera.position = ft_vec3f_add (rt.camera.position, ft_vec3f_mulf (right, (is_key_down ('D') - is_key_down ('A')) * speed));
		rt.camera.position = ft_vec3f_add (rt.camera.position, ft_vec3f_mulf (up, (is_key_down ('E') - is_key_down ('Q')) * speed));
		rt.camera.position = ft_vec3f_add (rt.camera.position, ft_vec3f_mulf (forward, (is_key_down ('W') - is_key_down ('S')) * speed));

		rt.camera.transform = ft_mat4f_rotate (ft_vec3f (1, 0, 0), rt.camera.pitch * PI / 180.0f);
		rt.camera.transform = ft_mat4f_mul (ft_mat4f_rotate (ft_vec3f (0, 1, 0), rt.camera.yaw * PI / 180.0f), rt.camera.transform);
		rt.camera.transform = ft_mat4f_mul (ft_mat4f_translate (rt.camera.position), rt.camera.transform);

		rt.camera.aspect_ratio = rt.camera.width / rt.camera.height;

		rt.light_position = ft_vec3f (cosf (seconds * 0.6) * 10, -100 + sinf (seconds * 0.5) * 10, cosf (seconds * 0.2) * 10);
		planet->sphere.center = ft_vec3f (cosf (seconds * 0.3) * 200, 0, sinf (seconds * 0.3) * 200);

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
