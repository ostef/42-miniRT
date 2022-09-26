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
	hit = raycast_closest (rt, ray);
	if (hit.object)
	{
		t_vec4f	color;
		if (rt->selected_object == hit.object)
		{
			diffuse_intensity = 1;
			color = ft_vec4f (1, 1, 1, 1);
		}
		else
		{
			t_vec3f	point_to_light = ft_vec3f_normalized (ft_vec3f_sub (rt->light_position, hit.point));
			ray.origin = hit.point;
			ray.dir = point_to_light;
			shadow_hit = raycast_closest_except (rt, ray, hit.object);
			if (shadow_hit.hit && shadow_hit.dist < ft_vec3f_dist (rt->light_position, ray.origin))
				diffuse_intensity = 0;
			else
				diffuse_intensity = ft_maxf (ft_vec3f_dot (hit.normal, point_to_light), 0);
			color = hit.object->color;
		}

		t_vec4f	diffuse = ft_vec4f_mulf (rt->light_color, rt->light_color.w * diffuse_intensity);
		t_vec4f	ambient = ft_vec4f_mulf (rt->ambient_light, rt->ambient_light.w);
		t_vec4f	light = ft_vec4f_add (diffuse, ambient);
		color.r *= light.r;
		color.g *= light.g;
		color.b *= light.b;
		set_pixel (&rt->win, px_x, px_y, color);
	}
}

t_plane	sphere_to_plane(t_sphere sph)
{
	t_plane	res;

	res.origin = sph.center;
	res.normal = ft_vec3f (0, -1, 0);
	return (res);
}

t_cylinder	sphere_to_cylinder(t_sphere sph)
{
	t_cylinder	res;

	res.bottom = ft_vec3f_sub(sph.center, ft_vec3f(0, -sph.radius * 2, 0));
	res.top = ft_vec3f_add(sph.center, ft_vec3f(0, -sph.radius * 2, 0));
	res.radius = sph.radius;
	return (res);
}

t_sphere	cylinder_to_sphere(t_cylinder cyl)
{
	t_sphere	res;

	res.center = ft_vec3f_mulf (ft_vec3f_add (cyl.top, cyl.bottom), 0.5f);
	res.radius = cyl.radius;
	return (res);
}

t_plane	cylinder_to_plane(t_cylinder cyl)
{
	t_plane	res;

	res.origin = ft_vec3f_mulf (ft_vec3f_add (cyl.top, cyl.bottom), 0.5f);
	res.normal = ft_vec3f_normalized (ft_vec3f_sub (cyl.top, cyl.bottom));
	return (res);
}

t_sphere	plane_to_sphere(t_plane pla)
{
	t_sphere	res;

	res.center = pla.origin;
	res.radius = 1;
	return (res);
}

t_cylinder	plane_to_cylinder(t_plane pla)
{
	t_cylinder	res;

	res.bottom = ft_vec3f_add (pla.origin, pla.normal);
	res.top = ft_vec3f_sub (pla.origin, pla.normal);
	res.radius = 1;
	return (res);
}

int	tick(void *ptr)
{
	t_rt	*rt;
	int		x;
	int		y;

	rt = (t_rt *)ptr;
	y = 0;
	while (y < rt->win.frame_height)
	{
		x = 0;
		while (x < rt->win.frame_width)
		{
			set_pixel(&rt->win, x, y, ft_vec4f(0.5, 0.5, 0.5, 0));
			// set_pixel (&rt->win, x, y, ft_vec4f_mulf (rt->ambient_light, rt->ambient_light.w));
			x += 1;
		}
		y += 1;
	}
	if (rt->is_editing)
	{
		if (is_key_pressed (&rt->win, KEY_SPACE))
		{
			rt->is_editing = FALSE;
			rt->selected_object = NULL;
		}
		if (rt->obj_count > 0)
		{
			if (is_key_pressed (&rt->win, KEY_RIGHT) || is_key_pressed (&rt->win, KEY_UP))
			{
				if (!rt->selected_object)
					rt->selected_object = rt->objs;
				else if (rt->selected_object == rt->objs + rt->obj_count - 1)
					rt->selected_object = NULL;
				else
					rt->selected_object += 1;
			}
			if (is_key_pressed (&rt->win, KEY_LEFT) || is_key_pressed (&rt->win, KEY_DOWN))
			{
				if (!rt->selected_object)
					rt->selected_object = rt->objs + rt->obj_count - 1;
				else if (rt->selected_object == rt->objs)
					rt->selected_object = NULL;
				else
					rt->selected_object -= 1;
			}
		}
		if (is_key_pressed (&rt->win, KEY_PLUS))
		{
			rt->selected_object = add_sphere (rt, ft_vec3f(0,0,0), 1);
		}
		if (is_key_pressed (&rt->win, KEY_MINUS) && rt->selected_object)
		{
			remove_object (rt, rt->selected_object - rt->objs);
			if (rt->selected_object >= rt->objs + rt->obj_count)
				rt->selected_object -= 1;;
			if (rt->selected_object < rt->objs)
				rt->selected_object = NULL;
		}
		if (rt->selected_object)
		{
			if (is_key_pressed (&rt->win, 'P'))
			{
				if (rt->selected_object->shape == SPHERE)
				{
					rt->selected_object->cylinder = sphere_to_cylinder (rt->selected_object->sphere);
					rt->selected_object->shape = CYLINDER;
				}
				else if (rt->selected_object->shape == CYLINDER)
				{
					rt->selected_object->plane = cylinder_to_plane (rt->selected_object->cylinder);
					rt->selected_object->shape = PLANE;
				}
				else if (rt->selected_object->shape == PLANE)
				{
					rt->selected_object->sphere = plane_to_sphere (rt->selected_object->plane);
					rt->selected_object->shape = SPHERE;
				}
			}
			

			t_vec3f	translation = ft_vec3f (
					is_key_down (&rt->win, 'D') - is_key_down (&rt->win, 'A'),
					is_key_down (&rt->win, 'E') - is_key_down (&rt->win, 'Q'),
					is_key_down (&rt->win, 'W') - is_key_down (&rt->win, 'S')
				);
			translate_object (rt->selected_object, ft_mat4f_transform_vector (rt->camera.transform, translation));
			
			rotate_object (rt->selected_object, ft_vec3f (
				is_key_down (&rt->win, 'L') - is_key_down (&rt->win, 'J'),
				is_key_down (&rt->win, 'O') - is_key_down (&rt->win, 'U'),
				is_key_down (&rt->win, 'I') - is_key_down (&rt->win, 'K')
			));
		}
	}
	else
	{
		if (is_key_pressed (&rt->win, KEY_SPACE))
		{
			rt->is_editing = TRUE;
			rt->selected_object = NULL;
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

		rt->camera.transform = ft_mat4f_rotate_euler (ft_vec3f (rt->camera.yaw * PI / 180.0f, rt->camera.pitch * PI / 180.0f, 0));rt->camera.transform = ft_mat4f_mul (ft_mat4f_translate (rt->camera.position), rt->camera.transform);

		rt->camera.aspect_ratio = rt->camera.width / rt->camera.height;
	}

	render_frame (rt);
	update_window(&rt->win);
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
