/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 22:32:17 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/25 22:32:17 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	remove_object(t_rt *rt, t_s64 index)
{
	ft_assert(index >= 0 && index < rt->obj_count,
		"Invalid obj index %d.", index);
	while (index < rt->obj_count - 1)
	{
		rt->objs[index] = rt->objs[index + 1];
		index += 1;
	}
	rt->obj_count -= 1;
}
