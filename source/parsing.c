#include "miniRT.h"

// t_vec3f	get_camera_position(t_cstr content)
// {

// }

// t_vec3f	get_camera_direction(t_cstr content)
// {

// }

//TODO checker erreur format
//TODO checker erreur malloc

//TODO lumiere
//TODO lumiere ambiante

t_str	get_next_element(t_str line)
{
	t_str	element;
	t_uint	start;
	t_uint	len;

	start = 0;
	while (line[start] && ft_is_space(line[start]))
		start++;
	len = 0;
	while (line[start + len] && !ft_is_space(line[start + len]))
		len++;
	element = ft_alloc(len + 1, ft_heap());
	ft_strncpy(element, line + start, len);
	ft_memmove(line, line + start + len + 1, ft_strlen(line) - start - len - 1);
	return (element);
}

int		ft_pow(int nb, int power)
{
	int i;
	int result;

	i = 0;
	result = 1;
	while (i < power)
	{
		result = result * nb;
		i++;
	}
	return (result);
}

t_f32	ft_atof(t_cstr str)
{
	t_str	str_int;
	t_f32	result;
	t_s64	integer;
	t_uint	index_decimal;

	index_decimal = 0;
	while (str[index_decimal] && str[index_decimal] != '.')
		index_decimal++;
	str_int = ft_strdup(str, ft_heap());
	ft_memmove(str_int + index_decimal, str_int + index_decimal + 1, ft_strlen(str_int) - index_decimal - 1);
	ft_str_to_s64(str_int, &integer);
	ft_free(str_int, ft_heap());
	result = integer / ft_pow(10, index_decimal);
	return (result);
}

t_vec3f	parse_coordinate(t_str line)
{
	t_str	coordinate;
	t_pcstr	*split;
	t_vec3f	result;
	t_str	x;
	t_str	y;
	t_str	z;

	coordinate = get_next_element(line);
	ft_fprintln (STDERR, "element: %s", coordinate);
	split = ft_split(coordinate, ',', ft_heap());
	ft_fprintln (STDERR, "1: %s", split[0].data);
	ft_fprintln (STDERR, "2: %s", split[1].data);
	ft_fprintln (STDERR, "3: %s", split[2].data);
	ft_fprintln (STDERR, "test11");
	if (!split[0].data)
		ft_fprintln (STDERR, "split 0");
	if (!split[1].data)
		ft_fprintln (STDERR, "split 1");
	if (!split[2].data)
		ft_fprintln (STDERR, "split 2");
		
	x = ft_strndup(split[0].data, split[0].len, ft_heap());
	y = ft_strndup(split[1].data, split[1].len, ft_heap());
	z = ft_strndup(split[2].data, split[2].len, ft_heap());
	ft_fprintln (STDERR, "test12");
	ft_free(split, ft_heap());
	ft_free(coordinate, ft_heap());
	result = ft_vec3f(ft_atof(x), ft_atof(y), ft_atof(z));
	ft_fprintln (STDERR, "test13");
	ft_free(x, ft_heap());
	ft_free(y, ft_heap());
	ft_free(z, ft_heap());
	return (result);
}

t_f32	parse_float(t_str line)
{
	t_str	number_str;
	t_f32	result;

	number_str = get_next_element(line);
	result = ft_atof(number_str);
	ft_free(number_str, ft_heap());
	return (result);
}

t_vec4f	parse_color(t_str line) //TODO checker range [0, 255]
{
	t_str	color_str;
	t_pcstr	*split;
	t_vec4f	color;
	t_str	r_str;
	t_str	g_str;
	t_str	b_str;

	ft_memset(&color, 0, sizeof(t_vec4f));
	color_str = get_next_element(line);
	split = ft_split(color_str, ',', ft_heap());
	r_str = ft_strndup(split[0].data, split[0].len, ft_heap());
	g_str = ft_strndup(split[1].data, split[1].len, ft_heap());
	b_str = ft_strndup(split[2].data, split[2].len, ft_heap());
	ft_free(split, ft_heap());
	ft_free(color_str, ft_heap());
	color.x = ft_atof(r_str);
	color.y = ft_atof(g_str);
	color.z = ft_atof(b_str);
	ft_free(r_str, ft_heap());
	ft_free(g_str, ft_heap());
	ft_free(b_str, ft_heap());
	return (color);
}

t_sphere	parse_sphere(t_str line)
{
	t_sphere	sphere;

	ft_fprintln (STDERR, "test8");
	sphere.center =	parse_coordinate(line);
	ft_fprintln (STDERR, "test9");
	sphere.radius = parse_float(line) / 2;
	ft_fprintln (STDERR, "test10");
	return (sphere);
}

t_plane	parse_plane(t_str line)
{
	t_plane	plane;

	plane.point = parse_coordinate(line);
	plane.normal = parse_coordinate(line); // TODO checker range [-1,1]
	return (plane);
}

t_cylinder	parse_cylinder(t_str line)
{
	t_cylinder	cylinder;
	t_vec3f		coordinate;
	t_vec3f		direction;
	t_f32		diameter;
	t_f32		height;

	coordinate = parse_coordinate(line);
	direction = parse_coordinate(line); // TODO checker range [-1,1]
	diameter = parse_float(line);
	height = parse_float(line);
	cylinder.radius = diameter / 2;
	cylinder.top = ft_vec3f_add(coordinate, ft_vec3f_mulf(direction, height / 2));
	cylinder.bottom = ft_vec3f_sub(coordinate, ft_vec3f_mulf(direction, height / 2));
	return (cylinder);
}

void	parse_line(t_str line, t_rt *rt)
{
	t_str		id;
	t_object	*object;

	ft_fprintln (STDERR, "line: %s", line);
	ft_fprintln (STDERR, "test1");
	id = get_next_element(line);
	if (!id)
		return ;
	ft_fprintln (STDERR, "test2");
	ft_fprintln (STDERR, "id: %s", id);
	if ((ft_strcmp(id, "sp") == 0) || (ft_strcmp(id, "pl") == 0) || (ft_strcmp(id, "cy") == 0))
	{
		ft_fprintln (STDERR, "test3");
		object = add_object(rt);
		ft_fprintln (STDERR, "test4");
		if (ft_strcmp(id, "sp") == 0)
		{
			ft_fprintln (STDERR, "test5");
			object->shape = SPHERE;
			ft_fprintln (STDERR, "test6");
			object->sphere = parse_sphere(line);
			ft_fprintln (STDERR, "test7");
		}
		else if (ft_strcmp(id, "pl") == 0)
		{
			object->shape = PLANE;
			object->plane = parse_plane(line);
		}
		else if (ft_strcmp(id, "cy") == 0)
		{
			object->shape = CYLINDER;
			object->cylinder	= parse_cylinder(line);
		}
		object->color = parse_color(line);
	}
	else if (ft_strcmp(id, "C"))
	{
		rt->camera.position = parse_coordinate(line);
		rt->camera.direction = parse_coordinate(line); // TODO check range [-1, 1]
		rt->camera.fov_in_degrees = parse_float(line); // TODO check range [0, 180] // float ou int ?
	}
	ft_free(id, ft_heap());
	ft_free(line, ft_heap());
}

t_rt	parsing(t_str filename)
{
	t_rt	output;
	t_str	file_content;
	t_pcstr	*content_splited;
	t_uint	i;

	ft_memset(&output, 0, sizeof(t_rt));
	file_content = ft_read_entire_file(filename, ft_heap());
	content_splited = ft_split(file_content, '\n', ft_heap());
	i = 0;
	while (content_splited[i].data)
	{
		parse_line(ft_strndup(content_splited[i].data, content_splited[i].len, ft_heap()), &output);
		i++;
	}
	ft_free(content_splited, ft_heap());
	ft_free(file_content, ft_heap());
	return (output);
}