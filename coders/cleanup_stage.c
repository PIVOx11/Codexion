/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_stage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 09:37:21 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/09 11:05:42 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

void	malloc_clean(t_data *data)
{
	if (data->coders)
		free(data->coders);
	if (data->dongles)
		free(data->dongles);
}
void dongle_destroy(t_dongle *dongles, int counter)
{
	int		i;

	i = -1;
	while (++i < counter)
	{
		pthread_mutex_destroy(&dongles[i].dongle_mutex);
		pthread_mutex_destroy(&dongles[i].d_data);
		pthread_cond_destroy(&dongles[i].dongle_cond);
	}
}
void coder_mutex_destroy(t_coder *coder, int counter)
{
	int		i;

	i = -1;
	while (++i < counter)
		pthread_mutex_destroy(&coder[i].coder_mutex);
}
void	clean_data(t_data *data, int counter)
{
	if (counter > 0)
		pthread_mutex_destroy(&data->stop);
	if (counter > 1)
		pthread_mutex_destroy(&data->data_mutex);
	if (counter > 2)
		pthread_cond_destroy(&data->data_cond);
		
}

void	clean_resources(t_data *data)
{
	clean_data(data, 3);
	dongle_destroy(data->dongles, data->coder_count);
	coder_mutex_destroy(data->coders, data->coder_count);
	malloc_clean(data);
}