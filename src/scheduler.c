/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:36:29 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/19 14:41:59 by blidriss         ###   ########.fr       */
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
	dongle->is_taken = TRUE;
	pthread_mutex_unlock(&dongle->d_data);
	while (ft_gettime() - last_relais < coder->data->cold_down_t)
		usleep(10);
	safe_print("has taken a dongle", coder);
}

void	add_request(t_coder *coder, t_dongle *f, t_dongle *s)
{
	long	time;

	pthread_mutex_lock(&f->d_data);
	time = coder->compile_start_t;
	f->queue[f->queue_s].coder = coder;
	f->queue[f->queue_s].dead_line = time + coder->data->bornout_t;
	f->queue_s++;
	preorety(f);
	pthread_mutex_unlock(&f->d_data);
	pthread_mutex_lock(&s->d_data);
	time = coder->compile_start_t;
	s->queue[s->queue_s].coder = coder;
	s->queue[s->queue_s].dead_line = time + coder->data->bornout_t;
	s->queue_s++;
	preorety(s);
	pthread_mutex_unlock(&s->d_data);
}

int	try_take_dongle(t_dongle *dongle, t_coder *coder)
{
	if (dongle->is_taken || dongle->queue[0].coder != coder)
		return (FALSE);
	return (TRUE);
}
