/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:33:55 by ljourand          #+#    #+#             */
/*   Updated: 2022/09/22 17:02:53 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parsing.h"
#include <stdio.h>

t_error	parse_sphere(t_pcstr *line_split, t_rt *rt)
{
	t_object	*object;
	t_error		code;

	if (!line_split[1].data || !line_split[2].data || !line_split[3].data || line_split[4].data)
		return (FORMAT);
	object = add_object(rt);
	object->shape = SPHERE;
	code = parse_coordinate_f(ft_dup_split_str(line_split[1]), &object->sphere.center);
	if (code != OK)
		return (code);
	code = parse_float_f(ft_dup_split_str(line_split[2]), &object->sphere.radius);
	if (code != OK)
		return (code);
	object->sphere.radius /= 2;
	code = parse_color_f(ft_dup_split_str(line_split[3]), &object->color.rgb);
	if (code != OK)
		return (code);
	return (OK);
}

t_error	parse_plane(t_pcstr *line_split, t_rt *rt)
{
	t_object	*object;
	t_error		code;

	if (!line_split[1].data || !line_split[2].data || !line_split[3].data || line_split[4].data)
		return (FORMAT);
	object = add_object(rt);
	object->shape = PLANE;
	code = parse_coordinate_f(ft_dup_split_str(line_split[1]), &object->plane.origin);
	if (code != OK)
		return (code);
	code = parse_direction_f(ft_dup_split_str(line_split[2]), &object->plane.normal);
	if (code != OK)
		return (code);
	code = parse_color_f(ft_dup_split_str(line_split[3]), &object->color.rgb);
	return (code);
}

t_error	parse_cylinder(t_pcstr *line_split, t_rt *rt)
{
	t_object	*object;
	t_vec3f		coordinate;
	t_vec3f		direction;
	t_f32		diameter;
	t_f32		height;
	t_error		code;

	if (!line_split[1].data || !line_split[2].data || !line_split[3].data || !line_split[4].data || !line_split[5].data || line_split[6].data)
		return (FORMAT);
	object = add_object(rt);
	object->shape = CYLINDER;
	code = parse_coordinate_f(ft_dup_split_str(line_split[1]), &coordinate);
	if (code != OK)
		return (code);
	code = parse_direction_f(ft_dup_split_str(line_split[2]), &direction);
	if (code != OK)
		return (code);
	code = parse_float_f(ft_dup_split_str(line_split[3]), &diameter);
	if (code != OK)
		return (code);
	code = parse_float_f(ft_dup_split_str(line_split[4]), &height);
	if (code != OK)
		return (code);
	object->cylinder.radius = diameter / 2;
	object->cylinder.top = ft_vec3f_add(coordinate, ft_vec3f_mulf(direction, height / 2));
	object->cylinder.bottom = ft_vec3f_sub(coordinate, ft_vec3f_mulf(direction, height / 2));
	code = parse_color_f(ft_dup_split_str(line_split[5]), &object->color.rgb);
	return (code);
}

t_error	parse_camera(t_pcstr *line_split, t_camera *camera)
{
	t_vec3f			direction;
	t_error			code;
	static t_bool	first = TRUE;
	
	if (!first)
		return (DOUBLE_CAMERA);
	first = FALSE;
	if (!line_split[1].data || !line_split[2].data || !line_split[3].data || line_split[4].data)
		return (FORMAT);
	code = parse_coordinate_f(ft_dup_split_str(line_split[1]), &camera->position);
	if (code != OK)
		return (code);
	code = parse_direction_f(ft_dup_split_str(line_split[2]), &direction);
	if (code != OK)
		return (code);
	(void)direction;
	// camera->yaw = atan2f(direction.y, direction.x);
	// t_f32 distance = sqrt(direction.z * direction.z + direction.x * direction.x);
	// camera->pitch = asinf(direction.y / distance);
	code = parse_float_f(ft_dup_split_str(line_split[3]), &camera->fov_in_degrees);
	if (code != OK)
		return (code);
	if (camera->fov_in_degrees < 0 || camera->fov_in_degrees > 180)
		return (RANGE_FOV);
	camera->transform = ft_mat4f_identity();
	return (OK);
}

t_error	parse_ambient_light(t_pcstr *line_split, t_vec4f *light)
{
	t_error			code;
	static t_bool	first = TRUE;
	
	if (!first)
		return (DOUBLE_AMBIENT_LIGHT);
	first = FALSE;
	if (!line_split[1].data || !line_split[2].data || line_split[3].data)
		return (FORMAT);
	code = parse_float_f(ft_dup_split_str(line_split[1]), &light->w);
	if (code != OK)
		return (code);
	if (light->w < 0 || light->w > 1)
		return (RANGE_BRIGHTNESS);
	code = parse_color_f(ft_dup_split_str(line_split[2]), &light->rgb);
	return (code);
}

t_error	parse_light(t_pcstr *line_split, t_rt *rt)
{
	t_error			code;
	static t_bool	first = TRUE;
	
	if (!first)
		return (DOUBLE_LIGHT);
	first = FALSE;
	if (!line_split[1].data || !line_split[2].data || !line_split[3].data || line_split[4].data)
		return (FORMAT);
	code = parse_coordinate_f(ft_dup_split_str(line_split[1]), &rt->light_position);
	if (code != OK)
		return (code);
	code = parse_float_f(ft_dup_split_str(line_split[2]), &rt->light_color.w); //check range [0, 1]
	if (code != OK)
		return (code);
	code = parse_color_f(ft_dup_split_str(line_split[3]), &rt->light_color.rgb);
	return (code);
}

t_error	parse_line(t_str line, t_rt *rt)
{
	t_pcstr		*line_split;
	t_error		code;

	line_split = ft_split(line, ' ', ft_heap());
	if (!line_split)
		return (FORMAT);
	if (!line_split[0].data)
		return (FORMAT);
	if (ft_strncmp(line_split[0].data, "sp", line_split[0].len) == 0)
		code = parse_sphere(line_split, rt);
	else if (ft_strncmp(line_split[0].data, "pl", line_split[0].len) == 0)
		code = parse_plane(line_split, rt);
	else if (ft_strncmp(line_split[0].data, "cy", line_split[0].len) == 0)
		code = parse_cylinder(line_split, rt);
	else if (ft_strncmp(line_split[0].data, "C", line_split[0].len) == 0)
		code = parse_camera(line_split, &rt->camera);
	else if (ft_strncmp(line_split[0].data, "A", line_split[0].len) == 0)
		code = parse_ambient_light(line_split, &rt->ambient_light);
	else if (ft_strncmp(line_split[0].data, "L", line_split[0].len) == 0)
		code = parse_light(line_split, rt);
	else
		code = IDENTIFIER;
	ft_free(line, ft_heap());
	ft_free(line_split, ft_heap());
	return (code);
}

t_error	check_filename(t_cstr filename)
{
	t_cstr	extension;
	
	if (!filename)
		return (NO_FILENAME);
	extension = ft_strrchr(filename, '.');
	if (!extension)
		return (FILENAME);
	if (ft_strcmp(extension, ".rt") != 0)
		return (FILENAME);
	return (OK);
}

t_rt	parsing(t_str filename)
{
	t_rt	output;
	t_str	file_content;
	t_pcstr	*content_splited;
	t_uint	i;
	t_error	code;

	code = check_filename(filename);
	if (code != OK)
		ft_error(code, 0);
	ft_memset(&output, 0, sizeof(t_rt));
	file_content = ft_read_entire_file(filename, ft_heap());
	if (!file_content)
		ft_error(FILE_CONTENT, 0);
	content_splited = ft_split(file_content, '\n', ft_heap());
	if (!content_splited)
		ft_error(FILE_CONTENT, 0);
	i = 0;
	while (content_splited[i].data)
	{
		code = parse_line(ft_dup_split_str(content_splited[i]), &output);
		if (code != OK)
			ft_error(code, i + 1);
		i++;
	}
	ft_free(content_splited, ft_heap());
	ft_free(file_content, ft_heap());
	return (output);
}
