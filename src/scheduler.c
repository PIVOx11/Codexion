/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:36:29 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/16 11:02:50 by blidriss         ###   ########.fr       */
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

	pthread_mutex_lock(&f->d_data);
	// printf("=====C%d request D%d===== in slot %d\n", coder->id, f->dongle_id, 
	// 		f->heap_size + 1); // debug check :)
	if (f->heap_s < 2)
	{
		time = coder->compile_start_t;
		f->heap[f->heap_s].coder = coder;
		f->heap[f->heap_s].dead_line = time + coder->data->bornout_t;
		f->heap_s++;
	}
	pthread_mutex_unlock(&f->d_data);
	pthread_mutex_lock(&s->d_data);
	// printf("=====C%d request D%d===== in slot %d\n", coder->id, s->dongle_id, 
	// 		s->heap_size + 1); // debug check :)
	if (s->heap_s < 2)
	{
		time = coder->compile_start_t;
		s->heap[s->heap_s].coder = coder;
		s->heap[s->heap_s].dead_line = time + coder->data->bornout_t;
		s->heap_s++;
	}
	pthread_mutex_unlock(&s->d_data);
}

int	try_take_dongle(t_dongle *dongle, t_coder *coder)
{
	if (dongle->is_taken || dongle->heap[0].coder != coder)
		return (FALSE);
	return (TRUE);
}
