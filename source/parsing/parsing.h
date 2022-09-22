/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:03:52 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/22 16:57:59 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "miniRT.h"

typedef enum e_error {
	OK,
	NO_FILENAME,
	FILENAME,
	FILE_CONTENT,
	FORMAT,
	IDENTIFIER,
	RANGE_DIRECTION,
	RANGE_COLOR,
	RANGE_BRIGHTNESS,
	RANGE_FOV,
	FLOAT,
	FORMAT_DIRECTION,
	FORMAT_COORDINATE,
	FORMAT_COLOR,
	DOUBLE_CAMERA,
	DOUBLE_LIGHT,
	DOUBLE_AMBIENT_LIGHT
}	t_error;

// Parse info

t_error	parse_coordinate_f(t_str str, t_vec3f *result);
t_error	parse_direction_f(t_str str, t_vec3f *result);
t_error	parse_float_f(t_str str, t_f32 *result);
t_error	parse_color_f(t_str str, t_vec3f *color);

// Utils

t_str	ft_dup_split_str(t_pcstr split_str);
void	ft_error(t_error code, t_uint nb_line);
int		ft_pow(int nb, int power);
t_bool	ft_is_decimal_number(t_cstr str);
t_f32	ft_atof(t_cstr str);

#endif
