/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:03:52 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/23 15:22:23 by soumanso         ###   ########lyon.fr   */
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
