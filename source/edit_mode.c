/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_mode.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 15:07:51 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/24 15:07:51 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"


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

void	edit_mode_update(t_rt *rt)
{
	if (rt->obj_count > 0)
	{
		if (is_key_pressed (&rt->win, MOUSE_LEFT))
		{
			t_ray	ray;

			ft_println ("Mouse left pressed");
			ray = ray_from_screen_point (rt, get_mouse_pos (&rt->win), FALSE);
			rt->selected_object = raycast_closest (rt, ray).object;
		}
	}
	if (is_key_pressed (&rt->win, KEY_PLUS))
		rt->selected_object = add_sphere (rt, ft_vec3f(0,0,0), 1);
	if (rt->selected_object)
	{
		if (is_key_pressed (&rt->win, KEY_MINUS))
		{
			remove_object (rt, rt->selected_object - rt->objs);
			if (rt->selected_object >= rt->objs + rt->obj_count)
				rt->selected_object -= 1;;
			if (rt->selected_object < rt->objs)
				rt->selected_object = NULL;
		}
		if (is_key_pressed (&rt->win, KEY_P))
		{
			if (rt->selected_object->type == SPHERE)
			{
				rt->selected_object->cylinder = sphere_to_cylinder (rt->selected_object->sphere);
				rt->selected_object->type = CYLINDER;
			}
			else if (rt->selected_object->type == CYLINDER)
			{
				rt->selected_object->plane = cylinder_to_plane (rt->selected_object->cylinder);
				rt->selected_object->type = PLANE;
			}
			else if (rt->selected_object->type == PLANE)
			{
				rt->selected_object->sphere = plane_to_sphere (rt->selected_object->plane);
				rt->selected_object->type = SPHERE;
			}
		}
		t_vec3f	translation = ft_vec3f (
				is_key_down (&rt->win, KEY_D) - is_key_down (&rt->win, KEY_A),
				is_key_down (&rt->win, KEY_E) - is_key_down (&rt->win, KEY_Q),
				is_key_down (&rt->win, KEY_W) - is_key_down (&rt->win, KEY_S)
			);
		translate_object (rt->selected_object, ft_mat4f_transform_vector (rt->camera.transform, translation));
		rotate_object (rt->selected_object, ft_vec3f (
			is_key_down (&rt->win, KEY_L) - is_key_down (&rt->win, KEY_J),
			is_key_down (&rt->win, KEY_O) - is_key_down (&rt->win, KEY_U),
			is_key_down (&rt->win, KEY_I) - is_key_down (&rt->win, KEY_K)
		));
		scale_object (rt->selected_object, ft_vec2f (
			is_key_down (&rt->win, KEY_RIGHT) - is_key_down (&rt->win, KEY_LEFT),
			is_key_down (&rt->win, KEY_UP) - is_key_down (&rt->win, KEY_DOWN)
		));
	}
}
