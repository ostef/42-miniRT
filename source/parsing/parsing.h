/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:03:52 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/26 18:10:37 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "miniRT.h"

typedef enum e_error {
	ERR_OK,
	ERR_NO_FILENAME,
	ERR_FILENAME,
	ERR_FILE_CONTENT,
	ERR_FORMAT,
	ERR_IDENTIFIER,
	ERR_RANGE_DIRECTION,
	ERR_RANGE_COLOR,
	ERR_RANGE_BRIGHTNESS,
	ERR_RANGE_FOV,
	ERR_FLOAT,
	ERR_FORMAT_DIRECTION,
	ERR_FORMAT_COORDINATE,
	ERR_FORMAT_COLOR,
	ERR_DOUBLE_CAMERA,
	ERR_DOUBLE_LIGHT,
	ERR_DOUBLE_AMBIENT_LIGHT
}	t_error;

// Parse info

t_error	parse_coordinate(t_str str, t_vec3f *result, t_alloc arena);
t_error	parse_direction(t_str str, t_vec3f *result, t_alloc arena);
t_error	parse_float(t_str str, t_f32 *result);
t_error	parse_color(t_str str, t_vec3f *color, t_alloc arena);

// Parse objects

t_error	parse_sphere(t_pcstr *line_split, t_rt *rt, t_alloc arena);
t_error	parse_plane(t_pcstr *line_split, t_rt *rt, t_alloc arena);
t_error	parse_cylinder(t_pcstr *line_split, t_rt *rt, t_alloc arena);

// Parse elements

t_error	parse_camera(t_pcstr *line_split, t_camera *camera, t_alloc arena);
t_error	parse_ambient_light(t_pcstr *line_split, t_vec4f *light, t_alloc arena);
t_error	parse_light(t_pcstr *line_split, t_rt *rt, t_alloc arena);

// Utils

t_str	ft_dup_split_str(t_pcstr split_str, t_alloc arena);
void	ft_error(t_error code, t_uint nb_line);
t_uint	get_len_split(t_pcstr *split);

t_f32	ft_atof(t_cstr str);

#endif
