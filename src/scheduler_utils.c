/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 10:24:29 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/17 17:37:45 by blidriss         ###   ########.fr       */
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

void	preorety(t_dongle *dongle)
{
	t_request	*heap;
	t_request	*f;
	t_request	*s;

	if (dongle->heap_s != 2)
		return ;
	heap = dongle->heap;
	f = &heap[0];
	s = &heap[1];
	if (dongle->data->scheduler == EDF)
	{
		safe_print("swap happen", dongle->heap[0].coder);
		safe_print("swap happen", dongle->heap[1].coder);
		if (heap[0].dead_line > heap[1].dead_line)
		{
			heap[0] = *s;
			heap[1] = *f;
		}
	}
}
