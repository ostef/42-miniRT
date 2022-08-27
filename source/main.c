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

static inline void	set_pixel(t_window *win, int x, int y, t_u32 color)
{
	((t_u32 *)win->pixels)[y * win->frame_width + x] = color;
}

static inline void	clear_frame(t_window *win, t_u32 color)
{
	for (int i = 0; i < win->frame_width * win->frame_height; i += 1)
		((t_u32 *)win->pixels)[i] = color;
}

void forward_to_orthonormal_basis (t_vec3f forward, t_vec3f *out_right, t_vec3f *out_up)
{
	t_vec3f	right;
	t_vec3f	up;

	right = ft_vec3f_cross (ft_vec3f(0, 1, 0), forward);
	if (ft_approx_zero (ft_vec3f_dot (right, right), 0.000001f))
	{
		up = ft_vec3f_cross (forward, ft_vec3f(1, 0, 0));
		right = ft_vec3f_cross (up, forward);
	}
	else
		up = ft_vec3f_cross (right, forward);
	if (out_right)
		*out_right = ft_vec3f_normalized (right);
	if (out_up)
		*out_up = ft_vec3f_normalized (up);
}

t_vec3f	get_camera_right(t_rt *rt)
{
	t_vec3f	right;

	forward_to_orthonormal_basis (rt->camera_direction, &right, NULL);
	return (right);
}

t_vec3f	get_camera_up(t_rt *rt)
{
	t_vec3f	up;

	forward_to_orthonormal_basis (rt->camera_direction, NULL, &up);
	return (up);
}

#define	rgb(r, g, b) (((r) << 16) | ((g) << 8) | (b))

int	main(void)
{
	t_rt	rt;

	ft_memset (&rt, 0, sizeof (rt));
	rt.camera_direction = ft_vec3f(0, 0, 1);
	if (!create_window (&rt.win, "miniRT", 640, 480))
	{
		ft_fprintln (STDERR, "Could not create window.");
		return (1);
	}
	t_object	*obj = add_sphere (&rt, ft_vec3f (0, 10, 200), 100.0);
	obj->color = rgb(255, 0, 0);
	
	obj = add_sphere (&rt, ft_vec3f (10, -60, 150), 90.0);
	obj->color = rgb (0, 255, 0);
	
	obj = add_sphere (&rt, ft_vec3f (-35, -14, 400), 200.0);
	obj->color = rgb (0, 0, 255);
	while (rt.win.opened)
	{
		t_u64	start = GetTickCount64 ();

		poll_window_events (&rt.win);
		clear_frame (&rt.win, 0);

		rt.camera_position.x += (is_key_down ('D') - is_key_down ('A'));
		rt.camera_position.y += (is_key_down ('E') - is_key_down ('Q'));
		rt.camera_position.z += (is_key_down ('W') - is_key_down ('S'));

		t_vec3f	camera_right = get_camera_right (&rt);
		t_vec3f	camera_up = get_camera_up (&rt);
		t_vec3f	camera_plane = ft_vec3f_sub (
			ft_vec3f_sub (rt.camera_position,
				ft_vec3f_mulf (camera_right, rt.win.frame_width * 0.5f)
			), ft_vec3f_mulf (camera_up, rt.win.frame_height * 0.5f));

		for (int y = 0; y < rt.win.frame_height; y += 1)
		{
			for (int x = 0; x < rt.win.frame_width; x += 1)
			{
				t_ray	ray;
				ray.origin = ft_vec3f_add (camera_plane, ft_vec3f_add (ft_vec3f_mulf (camera_right, x), ft_vec3f_mulf (camera_up, y)));
				ray.dir = rt.camera_direction;
				t_f32	closest_distance = ft_inf32 ();
				t_s64	closest_object = -1;
				for (int i = 0; i < rt.obj_count; i += 1)
				{
					if (rt.objs[i].shape != SPHERE)
						continue ;
					t_f32	distance;
					if (ray_sphere_intersection (ray, rt.objs[i].sphere, &distance))
					{
						if (distance < closest_distance)
						{
							closest_distance = distance;
							closest_object = i;
						}
					}
				}
				if (closest_object != -1)
					set_pixel (&rt.win, x, y, rt.objs[closest_object].color);
			}
		}
		update_window (&rt.win);

		ft_println ("%u", GetTickCount64 () - start);
		Sleep (10);
	}
	destroy_window (&rt.win);
	return (0);
}
