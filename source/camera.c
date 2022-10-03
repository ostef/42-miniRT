/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 15:08:52 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/24 15:08:52 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_ray	ray_from_screen_point(t_rt *rt, t_vec2f point, t_bool invert_y)
{
	t_ray	ray;

	ray.origin = rt->camera.position;
	ray.dir.x = (2 * point.x / rt->camera.width - 1) * rt->camera.aspect_ratio
		* rt->camera.scale;
	if (invert_y)
		ray.dir.y = (1 - 2 * point.y / rt->camera.height) * rt->camera.scale;
	else
		ray.dir.y = (2 * point.y / rt->camera.height - 1) * rt->camera.scale;
	ray.dir.z = 1;
	ray.dir = ft_vec3f_normalized (ray.dir);
	ray.dir = ft_mat4f_transform_vector (rt->camera.transform, ray.dir);
	return (ray);
}

static t_vec3f	get_input_vec(t_window *win)
{
	t_vec3f	input;

	input.x = is_key_down (win, KEY_D) - is_key_down (win, KEY_A);
	input.y = is_key_down (win, KEY_E) - is_key_down (win, KEY_Q);
	input.z = is_key_down (win, KEY_W) - is_key_down (win, KEY_S);
	input = ft_vec3f_normalized (input);
	if (is_key_down (win, KEY_SHIFT))
		input = ft_vec3f_mulf (input, 10);
	return (input);
}

void	update_camera_movement(t_rt *rt)
{
	t_vec3f	right;
	t_vec3f	up;
	t_vec3f	forward;
	t_vec3f	input;

	right = ft_mat4f_right_vector (rt->camera.transform);
	up = ft_mat4f_up_vector (rt->camera.transform);
	forward = ft_mat4f_forward_vector (rt->camera.transform);
	rt->camera.yaw += (is_key_down (&rt->win, KEY_RIGHT)
			- is_key_down (&rt->win, KEY_LEFT)) * 2;
	rt->camera.pitch += (is_key_down (&rt->win, KEY_UP)
			- is_key_down (&rt->win, KEY_DOWN)) * 2;
	rt->camera.pitch = ft_clampf (rt->camera.pitch, -80, 80);
	input = get_input_vec(&rt->win);
	rt->camera.position = ft_vec3f_add (rt->camera.position,
			ft_vec3f_mulf (right, input.x));
	rt->camera.position = ft_vec3f_add (rt->camera.position,
			ft_vec3f_mulf (up, input.y));
	rt->camera.position = ft_vec3f_add (rt->camera.position,
			ft_vec3f_mulf (forward, input.z));
	rt->camera.transform = ft_mat4f_rotate_euler (ft_vec3f
			(rt->camera.yaw * PI / 180.0f, rt->camera.pitch * PI / 180.0f, 0));
	rt->camera.transform = ft_mat4f_mul (ft_mat4f_translate
			(rt->camera.position), rt->camera.transform);
}
