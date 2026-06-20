/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 10:24:29 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/19 16:05:19 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_order(t_coder *coder, t_dongle **f, t_dongle **s)
{
	if (coder->id % 2 == 0)
	{
		*f = coder->right_dongle;
		*s = coder->left_dongle;
		return ;
	}
	*f = coder->left_dongle;
	*s = coder->right_dongle;
}

void	remove_request(t_coder *coder, t_dongle *dongle)
{
	int		i;

	pthread_mutex_lock(&dongle->d_data);
	i = -1;
	while (++i < dongle->queue_s)
	{
		if (dongle->queue[i].coder == coder)
		{
			dongle->queue[i] = dongle->queue[1];
			break ;
		}
	}
	dongle->queue_s--;
	pthread_mutex_unlock(&dongle->d_data);
}

void	preorety(t_dongle *dongle)
{
	t_request	*f;
	t_request	*s;

	if (dongle->queue_s != 2)
		return ;
	f = &dongle->queue[0];
	s = &dongle->queue[1];
	if (dongle->data->scheduler == EDF)
	{
		if (dongle->queue[0].dead_line > dongle->queue[1].dead_line)
		{
			dongle->queue[0] = *s;
			dongle->queue[1] = *f;
		}
	}
}
