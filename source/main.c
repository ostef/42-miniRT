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

	add_sphere (&rt, ft_vec3f (0, 1, 20), 10.0)->color = rgb(255, 0, 0);
	add_sphere (&rt, ft_vec3f (1, -6, 15), 9.0)->color = rgb (0, 255, 0);
	add_sphere (&rt, ft_vec3f (-3.5, -1.4, 40), 20.0)->color = rgb (0, 0, 255);
	
	float scale = tanf (60.0f * 0.5f * PI / 180.0f);
	while (rt.win.opened)
	{
		t_u64	start = GetTickCount64 ();

		poll_window_events (&rt.win);
		clear_frame (&rt.win, 0);

		rt.camera_position.x += (is_key_down ('D') - is_key_down ('A')) * 1;
		rt.camera_position.y += (is_key_down ('E') - is_key_down ('Q')) * 1;
		rt.camera_position.z += (is_key_down ('W') - is_key_down ('S')) * 1;
	
		float image_aspect_ratio = rt.win.frame_width / (float)rt.win.frame_height;
		float inv_width = 1 / (float)rt.win.frame_width;
		float inv_height = 1 / (float)rt.win.frame_height;
		t_mat4f	view_matrix = ft_mat4f_look_at(rt.camera_position, rt.camera_direction);

		t_ray	ray;
		ray.origin = ft_mat4f_transform_point (view_matrix, rt.camera_position);
		for (int y = 0; y < rt.win.frame_height; y += 1)
		{
			float yy = (1 - 2 * (y + 0.5) * inv_height) * scale;
			for (int x = 0; x < rt.win.frame_width; x += 1)
			{
				float xx = (2 * (x + 0.5) * inv_width - 1) * image_aspect_ratio * scale;

				ray.dir = ft_mat4f_transform_vector (view_matrix, ft_vec3f_normalized (ft_vec3f (xx, yy, 1)));
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
