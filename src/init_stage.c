/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:55:54 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/17 10:07:41 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coders_init(t_data *data)
{
	int		i;
	t_coder	*coder;

	i = -1;
	while (++i < data->coder_c)
	{
		coder = data->coders + i;
		if (pthread_mutex_init(&coder->coder_mutex, NULL))
			return (coder_mutex_destroy(data->coders, i), FALSE);
		coder->id = i + 1;
		coder->compile_c = 0;
		coder->left_dongle = &data->dongles[i];
		coder->right_dongle = &data->dongles[(i + 1) % data->coder_c];
		coder->data = data;
		coder->compile_start_t = 1;
		coder->finish = FALSE;
	}
	return (TRUE);
}

static int	dongles_init(t_data *data)
{
	int			i;
	t_dongle	*dongle;

	i = -1;
	while (++i < data->coder_c)
	{
		dongle = &data->dongles[i];
		dongle->id = i + 1;
		dongle->relais_t = 1;
		dongle->data = data;
		dongle->heap_s = 0;
		dongle->is_taken = FALSE;
		if (pthread_mutex_init(&dongle->d_data, NULL)
			||pthread_cond_init(&dongle->dongle_cond, NULL))
			return (dongle_destroy(data->dongles, i), FALSE);
	}
	return (TRUE);
}

static int	data_init(t_data *data)
{
	data->compiles = 0;
	data->semulation_over = FALSE;
	data->coders = malloc(sizeof(t_coder) * data->coder_c);
	data->dongles = malloc(sizeof(t_dongle) * data->coder_c);
	if (!data->coders || !data->dongles)
		return (malloc_clean(data), FALSE);
	return (TRUE);
}

int	full_init(t_data *data)
{
	if (!data_init(data))
		return (FALSE);
	if (pthread_mutex_init(&data->stop, NULL))
		return (malloc_clean(data), FALSE);
	if (pthread_mutex_init(&data->data_mutex, NULL))
		return (malloc_clean(data), clean_data(data, 1), FALSE);
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (malloc_clean(data), clean_data(data, 2), FALSE);
	if (!dongles_init(data))
		return (malloc_clean(data), clean_data(data, 3), FALSE);
	if (!coders_init(data))
		return (malloc_clean(data), clean_data(data, 3),
			dongle_destroy(data->dongles,
				data->coder_c),
			FALSE
		);
	return (TRUE);
}
