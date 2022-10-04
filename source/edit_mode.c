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

static void	change_object_type2(t_object *obj, t_object prev)
{
	if (obj->type == PLANE)
	{
		obj->light.pos = prev.plane.origin;
		obj->type = LIGHT;
	}
	else if (obj->type == LIGHT)
	{
		obj->sphere.center = prev.light.pos;
		obj->sphere.radius = 1;
		obj->type = SPHERE;
	}
}

static void	change_object_type(t_object *obj)
{
	t_object	prev;

	prev = *obj;
	if (obj->type == SPHERE)
	{
		obj->cylinder.bottom = ft_vec3f_sub(prev.sphere.center,
				ft_vec3f(0, -prev.sphere.radius * 2, 0));
		obj->cylinder.top = ft_vec3f_add(prev.sphere.center,
				ft_vec3f(0, -prev.sphere.radius * 2, 0));
		obj->cylinder.radius = prev.sphere.radius;
		obj->type = CYLINDER;
	}
	else if (obj->type == CYLINDER)
	{
		obj->plane.origin = ft_vec3f_mulf (ft_vec3f_add (
					prev.cylinder.top, prev.cylinder.bottom), 0.5f);
		obj->plane.normal = ft_vec3f_normalized (ft_vec3f_sub (
					prev.cylinder.top, prev.cylinder.bottom));
		obj->type = PLANE;
	}
	else
		change_object_type2 (obj, prev);
}

static void	handle_object_addition_and_deletion(t_rt *rt)
{
	t_object	*new_obj;

	if (is_key_pressed (&rt->win, KEY_PLUS))
	{
		if (rt->selected_object)
		{
			new_obj = add_object (rt);
			*new_obj = *rt->selected_object;
			rt->selected_object = new_obj;
		}
		else
			rt->selected_object = add_sphere (rt, ft_vec3f(0, 0, 0), 1);
	}
	if (rt->selected_object && is_key_pressed (&rt->win, KEY_MINUS))
	{
		remove_object (rt, rt->selected_object - rt->objs);
		if (rt->selected_object >= rt->objs + rt->obj_count)
			rt->selected_object -= 1;
		if (rt->selected_object < rt->objs)
			rt->selected_object = NULL;
	}
}

static void	edit_selected_object(t_rt *rt)
{
	t_vec3f		translation;

	if (is_key_pressed (&rt->win, KEY_T))
		change_object_type (rt->selected_object);
	if (rt->is_picking_color)
		rt->selected_object->color = update_color_picker (rt,
				rt->selected_object->color, rt->selected_object->type == LIGHT);
	translation = ft_vec3f (
			is_key_down (&rt->win, KEY_D) - is_key_down (&rt->win, KEY_A),
			is_key_down (&rt->win, KEY_E) - is_key_down (&rt->win, KEY_Q),
			is_key_down (&rt->win, KEY_W) - is_key_down (&rt->win, KEY_S)
			);
	translate_object (rt->selected_object,
		ft_mat4f_transform_vector (rt->camera.transform, translation));
	rotate_object (rt->selected_object, ft_vec3f (
			is_key_down (&rt->win, KEY_O) - is_key_down (&rt->win, KEY_U),
			is_key_down (&rt->win, KEY_L) - is_key_down (&rt->win, KEY_J),
			is_key_down (&rt->win, KEY_I) - is_key_down (&rt->win, KEY_K)
			));
	scale_object (rt->selected_object, ft_vec2f (
			is_key_down (&rt->win, KEY_RIGHT)
			- is_key_down (&rt->win, KEY_LEFT),
			is_key_down (&rt->win, KEY_UP)
			- is_key_down (&rt->win, KEY_DOWN)
			));
}

void	edit_mode_update(t_rt *rt)
{
	t_ray		ray;

	if (is_key_pressed (&rt->win, KEY_C))
		rt->is_picking_color = !rt->is_picking_color;
	handle_object_addition_and_deletion (rt);
	if (rt->selected_object)
		edit_selected_object (rt);
	else if (rt->is_picking_color)
		rt->ambient_light = update_color_picker (rt, rt->ambient_light, TRUE);
	if (rt->active_ui_elem)
		rt->ui_captured_mouse = TRUE;
	if (rt->obj_count > 0)
	{
		if (!rt->ui_captured_mouse && is_key_pressed (&rt->win, MOUSE_LEFT))
		{
			ray = ray_from_screen_point (rt, get_mouse_pos (&rt->win), FALSE);
			rt->selected_object = raycast_closest (rt, ray, FIL_ALL).object;
		}
	}
}
