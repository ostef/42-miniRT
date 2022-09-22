/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 17:33:03 by soumanso          #+#    #+#             */
/*   Updated: 2022/09/20 17:33:03 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_thread	create_thread(int (*f)(void *), void *data)
{
	return (CreateThread (NULL, 0, f, data, 0, NULL));
}

void	destroy_thread(t_thread t)
{
	CloseHandle (t);
}

void	wait_for_threads(t_thread *t, t_s64 n)
{
	WaitForMultipleObjects(n, t, TRUE, INFINITE);
}
