/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:36:29 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/09 21:45:55 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->d_data);
	while (!try_take_dongle(dongle, coder))
		pthread_cond_wait(&dongle->dongle_cond, &dongle->d_data);
	while (ft_gettime() - dongle->last_relais < coder->data->cold_down_time)
		usleep(500);
	dongle->is_taken = TRUE;
	pthread_mutex_unlock(&dongle->d_data);
	safe_print("has take a dongle", coder);
}

void	add_request(t_coder *coder)
{
	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		fill_request(coder, coder->left_dongle, coder->right_dongle);
		return ;
	}
	fill_request(coder, coder->right_dongle, coder->left_dongle);
}

int	try_take_dongle(t_dongle *dongle, t_coder *coder)
{
	if (dongle->is_taken || get_winner(dongle) != coder)
		return (FALSE);
	return (TRUE);
}

t_coder	*get_winner(t_dongle *dongle)
{
	if (dongle->heap_size == 1)
		return (dongle->heap[0].coder);
	if (dongle->data->scheduler == EDF)
	{
		if (dongle->heap[0].dead_line < dongle->heap[1].dead_line)
			return (dongle->heap[0].coder);
		else
			return (dongle->heap[1].coder);
	}
	return (dongle->heap[0].coder);
}
