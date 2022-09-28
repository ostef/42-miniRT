/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 11:05:27 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/25 11:05:27 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <math.h>
# include "libft.h"
# include "ft_math.h"
# ifdef FT_WIN32
#  include "win32_layer.h"
# else
#  include "minilibx_layer.h"
# endif

enum
{
	KS_UP = 0,
	KS_PRESSED = 1,
	KS_DOWN = 2,
	KS_RELEASED = 3
};

t_bool		create_window(t_window *win, t_cstr title, int width, int height);
void		update_window(t_window *win);
void		destroy_window(t_window *win);
void		main_loop(t_window *win, int (*f)(void *), void *data);
void		set_pixel(t_window *win, int x, int y, t_vec4f color);
t_vec4f		get_pixel(t_window *win, int x, int y);

int			create_thread(t_thread *thread, int (*f)(void *), void *data);
void		destroy_thread(t_thread t);
void		wait_for_threads(t_thread *t, t_s64 n);

t_vec2f		get_mouse_pos(t_window *win);
t_bool		is_key_down(t_window *win, t_key key);
t_bool		is_key_pressed(t_window *win, t_key key);
t_bool		is_key_released(t_window *win, t_key key);

typedef enum e_obj_type
{
	UNKNOWN_SHAPE = 0,
	SPHERE,
	CYLINDER,
	PLANE,
	LIGHT
}	t_obj_type;

typedef struct s_sphere
{
	t_vec3f	center;
	t_f32	radius;
}	t_sphere;

typedef struct s_cylinder
{
	t_vec3f	top;
	t_vec3f	bottom;
	t_f32	radius;
}	t_cylinder;

typedef struct s_plane
{
	t_vec3f	origin;
	t_vec3f	normal;
}	t_plane;

typedef struct s_light
{
	t_vec3f	pos;
}	t_light;

typedef struct s_object
{
	t_obj_type		type;
	union
	{
		t_sphere	sphere;
		t_cylinder	cylinder;
		t_plane		plane;
		t_light		light;
	};
	t_vec4f			color;
}	t_object;

typedef struct s_ray
{
	t_vec3f	origin;
	t_vec3f	dir;
}	t_ray;

typedef struct s_camera
{
	t_vec3f	position;
	t_f32	yaw;
	t_f32	pitch;
	t_mat4f	transform;
	t_f32	fov_in_degrees;
	t_f32	scale;
	t_f32	width;
	t_f32	height;
	t_f32	aspect_ratio;
}	t_camera;

typedef struct s_color_picker
{
	t_bool	opened;
	t_bool	has_alpha;
	t_vec4f	color;
}	t_color_picker;

typedef enum e_ui_id
{
	UI_NONE = 0,
	UI_COLOR_RING,
	UI_COLOR_TRIANGLE,
	UI_COLOR_ALPHA
}	t_ui_id;

typedef struct s_rt
{
	t_window		win;
	t_camera		camera;
	t_vec4f			ambient_light;
	t_object		*objs;
	t_s64			obj_count;
	t_s64			obj_cap;
	t_bool			is_editing;
	t_object		*selected_object;
	t_bool			is_picking_color;
	t_bool			ui_captured_mouse;
	t_color_picker	color_picker;
	t_ui_id			active_ui_elem;
	t_bool			keep_ui_elem_active;
}	t_rt;

t_object	*add_object(t_rt *rt);
t_object	*add_sphere(t_rt *rt, t_vec3f center, t_f32 radius);
t_object	*add_cylinder(t_rt *rt, t_vec3f bottom, t_vec3f top, t_f32 radius);
t_object	*add_plane(t_rt *rt, t_vec3f origin, t_vec3f normal);
t_object	*add_light(t_rt *rt, t_vec3f position, t_vec4f color);
void		remove_object(t_rt *rt, t_s64 index);

void		translate_object(t_object *obj, t_vec3f amount);
void		rotate_object(t_object *obj, t_vec3f amount);
void		scale_object(t_object *obj, t_vec2f amount);

t_rt		parsing(t_str filename);

void		edit_mode_update(t_rt *rt);
void		update_camera_movement(t_rt *rt);
t_ray		ray_from_screen_point(t_rt *rt, t_vec2f point, t_bool invert_t);

void		clear_frame(t_rt *rt, t_vec4f color);
void		render_pixel(t_rt *rt, t_int px_x, t_int px_y);
void		render_frame(t_rt *rt);

typedef enum e_filter
{
	FIL_NONE = 0x00,
	FIL_SPHERES = 1 << SPHERE,
	FIL_CYLINDERS = 1 << CYLINDER,
	FIL_PLANES = 1 << PLANE,
	FIL_SHAPES = FIL_SPHERES | FIL_CYLINDERS | FIL_PLANES,
	FIL_LIGHTS = 1 << LIGHT,
	FIL_ALL = 0xffffffff
}	t_filter;

typedef struct s_hit_res
{
	t_vec3f		normal;
	t_vec3f		point;
	t_f32		dist;
	t_object	*object;
	t_bool		hit;
}	t_hit_res;

t_bool		ray_sphere_intersection(t_ray ray, t_sphere sph, t_hit_res *res);
t_bool		ray_cylinder_intersection(t_ray ray, t_cylinder cyl,
				t_hit_res *res);
t_bool		ray_plane_intersection(t_ray ray, t_plane pla, t_hit_res *res);
t_bool		ray_object_intersection(t_ray ray, t_object *obj, t_hit_res *res);

t_hit_res	raycast_first(t_rt *rt, t_ray ray, t_filter fil);
t_hit_res	raycast_first_except(t_rt *rt, t_ray ray, t_object *ignore, t_filter fil);
t_hit_res	raycast_closest(t_rt *rt, t_ray ray, t_filter fil);
t_hit_res	raycast_closest_except(t_rt *rt, t_ray ray, t_object *ignore, t_filter fil);
void		render_pixel(t_rt *rt, t_int px_x, t_int px_y);

t_vec4f		update_color_picker(t_rt *rt, t_vec4f color, t_bool has_alpha);
void		draw_color_picker(t_rt *rt);

/* UI Utils */

t_bool	point_is_inside_circle(t_vec2f pt, t_vec2f center, t_f32 radius);
t_bool	point_is_inside_ring(t_vec2f pt, t_vec2f center, t_vec2f radius);
t_bool	point_is_inside_rect(t_vec2f p, t_rectf r);
t_bool	point_is_inside_triangle(t_vec2f p, t_vec2f t0, t_vec2f t1, t_vec2f t2);

/* 2D Drawing functions (used for UI elements) */

void	draw_pixel(t_rt *rt, int x, int y, t_vec4f color);
void	draw_rect(t_rt *rt, t_rectf r, t_vec4f color);
void	draw_circle(t_rt *rt, t_vec2f center, t_f32 radius, t_vec4f color);
void	draw_ring(t_rt *rt, t_vec2f center, t_vec2f radius, t_vec4f color);

#endif
