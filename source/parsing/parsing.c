/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljourand <ljourand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:33:55 by ljourand          #+#    #+#             */
/*   Updated: 2022/10/03 14:22:53 by ljourand         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parsing.h"

static t_error	parse_line(t_str line, t_rt *rt, t_alloc arena)
{
	t_pcstr		*line_split;
	t_error		code;

	line_split = ft_split(line, ' ', arena);
	if (!line_split)
		return (ERR_FORMAT);
	if (!line_split[0].data)
		return (ERR_FORMAT);
	if (ft_strncmp(line_split[0].data, "sp", line_split[0].len) == 0)
		code = parse_sphere(line_split, rt, arena);
	else if (ft_strncmp(line_split[0].data, "pl", line_split[0].len) == 0)
		code = parse_plane(line_split, rt, arena);
	else if (ft_strncmp(line_split[0].data, "cy", line_split[0].len) == 0)
		code = parse_cylinder(line_split, rt, arena);
	else if (ft_strncmp(line_split[0].data, "C", line_split[0].len) == 0)
		code = parse_camera(line_split, &rt->camera, arena);
	else if (ft_strncmp(line_split[0].data, "A", line_split[0].len) == 0)
		code = parse_ambient_light(line_split, &rt->ambient_light, arena);
	else if (ft_strncmp(line_split[0].data, "L", line_split[0].len) == 0)
		code = parse_light(line_split, rt, arena);
	else
		code = ERR_IDENTIFIER;
	return (code);
}

static t_error	check_filename(t_cstr filename)
{
	t_cstr	extension;

	if (!filename)
		return (ERR_NO_FILENAME);
	extension = ft_strrchr(filename, '.');
	if (!extension)
		return (ERR_FILENAME);
	if (ft_strcmp(extension, ".rt") != 0)
		return (ERR_FILENAME);
	return (ERR_OK);
}

static t_error	check_elements(t_pcstr *split)
{
	t_uint	i;
	t_bool	camera;
	t_bool	ambient;
	t_bool	light;

	camera = FALSE;
	ambient = FALSE;
	light = FALSE;
	i = -1;
	while (split[++i].data)
	{
		if (ft_strncmp(split[i].data, "C ", 2) == 0)
			camera = TRUE;
		else if (ft_strncmp(split[i].data, "A ", 2) == 0)
			ambient = TRUE;
		else if (ft_strncmp(split[i].data, "L ", 2) == 0)
			light = TRUE;
	}
	if (!camera)
		return (ERR_NO_CAMERA);
	if (!ambient)
		return (ERR_NO_AMBIENT);
	if (!light)
		return (ERR_NO_LIGHT);
	return (ERR_OK);
}

static void	parsing_the_return(t_str filename, t_rt *output, t_alloc arena)
{
	t_str	file_content;
	t_pcstr	*content_splited;
	t_uint	i;
	t_error	code;

	file_content = ft_read_entire_file(filename, arena);
	if (!file_content)
		ft_error(ERR_FILE_CONTENT, 0);
	content_splited = ft_split(file_content, '\n', arena);
	if (!content_splited)
		ft_error(ERR_FILE_CONTENT, 0);
	ft_error(check_elements(content_splited), 0);
	i = 0;
	while (content_splited[i].data)
	{
		code = parse_line(ft_dup_split_str(content_splited[i], arena),
				output, arena);
		ft_error(code, ft_count_chars(file_content, '\n',
				content_splited[i].data - file_content) + 1);
		i++;
	}
}

t_rt	parsing(t_str filename)
{
	t_arena		arena_storage;
	t_alloc		arena;
	t_rt		output;

	ft_init_arena(&arena_storage, 10000, ft_heap());
	arena = ft_arena(&arena_storage);
	ft_error(check_filename(filename), 0);
	ft_memset(&output, 0, sizeof(t_rt));
	parsing_the_return(filename, &output, arena);
	ft_free_arena(&arena_storage);
	return (output);
}
