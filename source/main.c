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

int	tick(void *ptr)
{
	t_rt	*rt;

	rt = (t_rt *)ptr;
	rt->camera.width = rt->win.frame_width;
	rt->camera.height = rt->win.frame_height;
	rt->camera.scale = tanf (rt->camera.fov_in_degrees * 0.5f * PI / 180.0f);
	rt->camera.aspect_ratio = rt->camera.width / rt->camera.height;
	if (is_key_pressed (&rt->win, KEY_SPACE))
	{
		rt->is_editing = !rt->is_editing;
		rt->selected_object = NULL;
	}
	ft_memset (&rt->color_picker, 0, sizeof (t_color_picker));
	if (!rt->keep_ui_elem_active)
		rt->active_ui_elem = UI_NONE;
	rt->keep_ui_elem_active = FALSE;
	rt->ui_captured_mouse = FALSE;
	if (rt->is_editing)
		edit_mode_update (rt);
	else
		update_camera_movement (rt);
	clear_frame (rt, ft_vec4f_mulf (rt->ambient_light, rt->ambient_light.w));
	render_frame (rt);
	draw_color_picker (rt);
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
