/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 10:24:29 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/13 14:32:08 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_order(t_coder *coder, t_dongle **f, t_dongle **s)
{
	if (coder->id % 2 == 0)
	{
		*f = coder->left_dongle;
		*s = coder->right_dongle;
		return ;
	}
	*f = coder->right_dongle;
	*s = coder->left_dongle;
}

void	remove_request(t_coder *coder, t_dongle *dongle)
{
	int		i;

	pthread_mutex_lock(&dongle->d_data);
	i = -1;
	while (++i < dongle->heap_s)
	{
		if (dongle->heap[i].coder == coder)
		{
			dongle->heap[i] = dongle->heap[1];
			break ;
		}
	}
	dongle->heap_s--;
	pthread_mutex_unlock(&dongle->d_data);
}

void	fill_request(t_coder *coder, t_dongle *f, t_dongle *s)
{
	long	time;

	pthread_mutex_lock(&f->d_data);
	if (f->heap_s < 2)
	{
		time = coder->compile_start_t;
		f->heap[f->heap_s].coder = coder;
		f->heap[f->heap_s].dead_line = time + coder->data->bornout_t;
		f->heap_s++;
		if (f->heap_s == 2)
			preorety(f);
	}
	pthread_mutex_unlock(&f->d_data);
	pthread_mutex_lock(&s->d_data);
	if (s->heap_s < 2)
	{
		time = coder->compile_start_t;
		s->heap[s->heap_s].coder = coder;
		s->heap[s->heap_s].dead_line = time + coder->data->bornout_t;
		s->heap_s++;
		if (s->heap_s == 2)
			preorety(s);
	}
	pthread_mutex_unlock(&s->d_data);
}

void	preorety(t_dongle *dongle)
{
	t_request	*heap;
	t_request	*f;
	t_request	*s;

	heap = dongle->heap;
	f = &heap[0];
	s = &heap[1];
	if (dongle->data->scheduler == EDF)
	{
		if (heap[0].dead_line > heap[1].dead_line)
		{
			heap[0] = *s;
			heap[1] = *f;
		}
	}
}
