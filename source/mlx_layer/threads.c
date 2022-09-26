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

void	*layer(void *th)
{
	t_thread	*thread;

	thread = (t_thread *)th;
	thread->routine(thread->data);
	return (NULL);
}

int	create_thread(t_thread *thread, int (*f)(void *), void *data)
{
	thread->routine = f;
	thread->data = data;
	return (pthread_create(&thread->thread, 0, layer, thread));
}

void	destroy_thread(t_thread t)
{
	pthread_detach(t.thread);
}

void	wait_for_threads(t_thread *t, t_s64 n)
{
	t_uint	i;

	i = 0;
	while (i < n)
	{
		pthread_join(t[i].thread, 0);
		i++;
	}
}
