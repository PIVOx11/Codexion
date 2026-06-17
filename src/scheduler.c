/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:36:29 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/17 16:58:16 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_dongle(t_coder *coder, t_dongle *dongle)
{
	long	last_relais;

	pthread_mutex_lock(&dongle->d_data);
	while (!try_take_dongle(dongle, coder))
		pthread_cond_wait(
			&dongle->dongle_cond,
			&dongle->d_data);
	last_relais = dongle->relais_t;
	pthread_mutex_unlock(&dongle->d_data);
	while (ft_gettime() - last_relais < coder->data->cold_down_t)
		usleep(500);
	pthread_mutex_lock(&dongle->d_data);
	dongle->is_taken = TRUE;
	pthread_mutex_unlock(&dongle->d_data);
	safe_print("has taken a dongle", coder);
}

void	add_request(t_coder *coder, t_dongle *f, t_dongle *s)
{
	long	time;

// 	printf(
//     "%ld C%d requests D%d,D%d\n",
//     ft_gettime() - coder->data->semulation_start,
//     coder->id,
//     f->id,
//     s->id
// );
	pthread_mutex_lock(&f->d_data);
	if (f->heap_s < 2)
	{
		time = coder->compile_start_t;
		f->heap[f->heap_s].coder = coder;
		f->heap[f->heap_s].dead_line = time + coder->data->bornout_t;
		f->heap_s++;
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
		preorety(s);
	}
	pthread_mutex_unlock(&s->d_data);
}

int	try_take_dongle(t_dongle *dongle, t_coder *coder)
{
	if (dongle->is_taken || dongle->heap[0].coder != coder)
		return (FALSE);
	return (TRUE);
}
