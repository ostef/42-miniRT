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

t_bool	render_sphere_for_light(t_rt *rt, t_int px_x, t_int px_y, t_ray ray)
{
	t_sphere	sph;

	sph.center = rt->light_position;
	sph.radius = 1;
	if (ray_sphere_intersection (ray, sph, NULL))
	{
		set_pixel (&rt->win, px_x, px_y, rt->light_color);
		return (TRUE);
	}
	return (FALSE);
}

void	render_pixel(t_rt *rt, t_int px_x, t_int px_y)
{
	t_ray		ray;
	t_hit_res	hit;
	t_hit_res	shadow_hit;
	t_f32		diffuse_intensity;

	ray.origin = rt->camera.position;
	ray.dir = ft_vec3f (
		(2 * (px_x + 0.5f) / rt->camera.width - 1) * rt->camera.aspect_ratio * rt->camera.scale,
		(1 - 2 * (px_y + 0.5f) / rt->camera.height) * rt->camera.scale, 1);
	ray.dir = ft_mat4f_transform_vector(rt->camera.transform, ft_vec3f_normalized (ray.dir));
	//if (render_sphere_for_light (rt, px_x, px_y, ray))
	//	return ;
	hit = raycast_closest (rt, ray);
	if (hit.object)
	{
		t_vec3f	point_to_light = ft_vec3f_normalized (ft_vec3f_sub (rt->light_position, hit.point));
		ray.origin = hit.point;
		ray.dir = point_to_light;
		shadow_hit = raycast_closest_except (rt, ray, hit.object);
		if (shadow_hit.hit && shadow_hit.dist < ft_vec3f_dist (rt->light_position, ray.origin))
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
		set_pixel (&rt->win, px_x, px_y, color);
	}
}

typedef struct s_thread_data
{
	t_rt	*rt;
	t_int	x0;
	t_int	y0;
	t_int	x1;
	t_int	y1;
}	t_thread_data;

DWORD	render_thread(void *param)
{
	t_thread_data	data;
	int				x;
	int				y;
	
	data = *(t_thread_data *)param;
	y = data.y0;
	while (y < data.y1)
	{
		x = data.x0;
		while (x < data.x1)
		{
			render_pixel (data.rt, x, y);
			x += 1;
		}
		y += 1;
	}
	return (0);
}

#define THREAD_X_COUNT (4)
#define THREAD_Y_COUNT (4)
#define THREAD_COUNT (THREAD_X_COUNT * THREAD_Y_COUNT)

int	tick(t_rt *rt)
{

	int	x;
	int	y;

	y = 0;
	while (y < rt->win.frame_height)
	{
		x = 0;
		while (x < rt->win.frame_width)
		{
			set_pixel (&rt->win, x, y, ft_vec4f_mulf (rt->ambient_light, rt->ambient_light.w));
			x += 1;
		}
		y += 1;
	}

	rt->camera.width = rt->win.frame_width;
	rt->camera.height = rt->win.frame_height;
	rt->camera.scale = tanf (rt->camera.fov_in_degrees * 0.5f * PI / 180.0f);
	t_f32	speed = 1;
	if (is_key_down (&rt->win, KEY_SHIFT))
		speed = 10;

	t_vec3f	right = ft_mat4f_right_vector (rt->camera.transform);
	t_vec3f	up = ft_mat4f_up_vector (rt->camera.transform);
	t_vec3f	forward = ft_mat4f_forward_vector (rt->camera.transform);

	rt->camera.yaw += (is_key_down (&rt->win, KEY_RIGHT) - is_key_down (&rt->win, KEY_LEFT)) * 2;
	rt->camera.pitch += (is_key_down (&rt->win, KEY_UP) - is_key_down (&rt->win, KEY_DOWN)) * 2;
	rt->camera.pitch = ft_clampf (rt->camera.pitch, -80, 80);

	rt->camera.position = ft_vec3f_add (rt->camera.position, ft_vec3f_mulf (right, (is_key_down (&rt->win, 'D') - is_key_down (&rt->win, 'A')) * speed));
	rt->camera.position = ft_vec3f_add (rt->camera.position, ft_vec3f_mulf (up, (is_key_down (&rt->win, 'E') - is_key_down (&rt->win, 'Q')) * speed));
	rt->camera.position = ft_vec3f_add (rt->camera.position, ft_vec3f_mulf (forward, (is_key_down (&rt->win, 'W') - is_key_down (&rt->win, 'S')) * speed));

	rt->camera.transform = ft_mat4f_rotate (ft_vec3f (1, 0, 0), rt->camera.pitch * PI / 180.0f);
	rt->camera.transform = ft_mat4f_mul (ft_mat4f_rotate (ft_vec3f (0, 1, 0), rt->camera.yaw * PI / 180.0f), rt->camera.transform);
	rt->camera.transform = ft_mat4f_mul (ft_mat4f_translate (rt->camera.position), rt->camera.transform);

	rt->camera.aspect_ratio = rt->camera.width / rt->camera.height;

	// rt->light_position = ft_vec3f (cosf (seconds * 0.6) * 10, -100 + sinf (seconds * 0.5) * 10, cosf (seconds * 0.2) * 10);
	// planet->sphere.center = ft_vec3f (cosf (seconds * 0.3) * 200, 0, sinf (seconds * 0.3) * 200);

	HANDLE	threads[THREAD_COUNT] = {0};
	t_thread_data	thread_data[THREAD_COUNT] = {0};

	for (int y = 0; y < THREAD_Y_COUNT; y += 1)
	{
		for (int x = 0; x < THREAD_X_COUNT; x += 1)
		{
			int i = y * THREAD_X_COUNT + x;
			thread_data[i].rt = rt;
			thread_data[i].x0 = x * rt->win.frame_width / THREAD_X_COUNT;
			thread_data[i].x1 = (x + 1) * rt->win.frame_width / THREAD_X_COUNT;
			thread_data[i].y0 = y * rt->win.frame_height / THREAD_Y_COUNT;
			thread_data[i].y1 = (y + 1) * rt->win.frame_height / THREAD_Y_COUNT;
			threads[i] = CreateThread (NULL, 0, render_thread, &thread_data[i], 0, NULL);
		}
	}

	WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE);

	for (int i = 0; i < THREAD_COUNT; i += 1)
	{
		CloseHandle (threads[i]);
	}

	/*
	y = 0;
	while (y < rt->win.frame_height)
	{
		x = 0;
		while (x < rt->win.frame_width)
		{
			render_pixel (rt, x, y);
			x += 1;
		}
		y += 1;
	}
	*/

	return (0);
}

int	main(int ac, char **av)
{
	t_rt	rt;

	if (ac < 2 || ac > 3)
		return (1);
	ft_memset (&rt, 0, sizeof (rt));
	rt = parsing(av[1]);
	if (!create_window (&rt.win, "miniRT", 640, 480))
	{
		ft_fprintln (STDERR, "Could not create window.");
		return (1);
	}
	main_loop (&rt.win, &tick, &rt);
	destroy_window (&rt.win);
	return (0);
}
