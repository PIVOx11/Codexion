/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_stage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 09:15:24 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/11 16:59:34 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	update_compile_state(t_coder *coder)
{
	safe_print("is compiling", coder);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->compile_count++;
	coder->last_compile_start = ft_gettime();
	if (coder->compile_count == coder->data->compile_req)
	{
		coder->finish = TRUE;
		pthread_mutex_lock(&coder->data->data_mutex);
		coder->data->compiles++;
		pthread_mutex_unlock(&coder->data->data_mutex);
	}
	pthread_mutex_unlock(&coder->coder_mutex);
}

static void	relaise_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->d_data);
	dongle->is_taken = FALSE;
	dongle->last_relais = ft_gettime();
	pthread_cond_broadcast(&dongle->dongle_cond);
	pthread_mutex_unlock(&dongle->d_data);
}

void	compile(t_coder *coder, t_dongle *first, t_dongle *second)
{
	add_request(coder, first, second);
	wait_dongle(coder, first);
	wait_dongle(coder, second);
	remove_request(coder, first);
	remove_request(coder, second);
	update_compile_state(coder);
	safe_sleep(coder, coder->data->compile_time);
	relaise_dongle(first);
	relaise_dongle(second);
}

void	debug(t_coder *coder)
{
	safe_print("is debugging", coder);
	safe_sleep(coder, coder->data->debug_time);
}

void	refactol(t_coder *coder)
{
	safe_print("is refactoring", coder);
	safe_sleep(coder, coder->data->refactor_time);
}
