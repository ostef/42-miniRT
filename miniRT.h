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

# include "libft.h"
# include "ft_math.h"
# ifdef FT_WIN32
#  include "win32_layer.h"
# endif

typedef enum e_shape
{
	UNKNOWN_SHAPE = 0,
	SPHERE,
	CYLINDER,
	PLANE
}	t_shape;

typedef struct s_sphere
{
	t_vec3f	center;
	t_f32	radius;
}	t_sphere;

typedef struct s_cylinder
{
	t_vec3f	bottom;
	t_vec3f	top;
	t_f32	radius;
}	t_cylinder;

typedef struct s_plane
{
	t_vec3f	point;
	t_vec3f	normal;
}	t_plane;

typedef struct s_object
{
	t_shape			shape;
	union
	{
		t_sphere	sphere;
		t_cylinder	cylinder;
		t_plane		plane;
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
	t_vec3f	direction;
	t_mat4f	transform;
	t_f32	fov_in_degrees;
	t_f32	scale;
	t_f32	width;
	t_f32	height;
	t_f32	aspect_ratio;
}	t_camera;

typedef struct s_rt
{
	t_window	win;
	t_camera	camera;
	t_vec3f		light_position;
	t_object	*objs;
	t_s64		obj_count;
	t_s64		obj_cap;
}	t_rt;

t_object	*add_object(t_rt *rt);
t_object	*add_sphere(t_rt *rt, t_vec3f center, t_f32 radius);
t_object	*add_cylinder(t_rt *rt, t_vec3f bottom, t_vec3f top, t_f32 radius);
t_object	*add_plane(t_rt *rt, t_vec3f point, t_vec3f normal);
void		remove_object(t_rt *rt, t_s64 index);


t_rt		parsing(t_str filename);

// t_bool		ray_sphere_intersection(t_ray ray, t_sphere sph, t_f32 *out_dist);
typedef struct s_hit_result
{
	t_vec3f		normal;
	t_vec3f		point;
	t_f32		dist;
	t_object	*object;
	t_bool		hit;
}	t_hit_result;

t_bool		ray_sphere_intersection(t_ray ray, t_sphere sph, t_hit_result *res);
t_bool		ray_object_intersection(t_ray ray, t_object *obj, t_hit_result *res);

t_hit_result	raycast_first(t_rt *rt, t_ray ray);
t_hit_result	raycast_first_except(t_rt *rt, t_ray ray, t_object *ignore);
t_hit_result	raycast_closest(t_rt *rt, t_ray ray);

#endif
