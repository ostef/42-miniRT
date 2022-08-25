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
	t_u32			color;
}	t_object;

typedef struct s_rt
{
	t_object	*objs;
	t_s64		obj_count;
	t_s64		obj_cap;
}	t_rt;

t_object	*add_object(t_rt *rt);
void		add_sphere(t_rt *rt, t_vec3f center, t_f32 radius);
void		add_cylinder(t_rt *rt, t_vec3f bottom, t_vec3f top, t_f32 radius);
void		add_plane(t_rt *rt, t_vec3f point, t_vec3f normal);
void		remove_object(t_rt *rt, t_s64 index);

#endif
