/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 21:12:48 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/09 21:12:49 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_burned_out(t_coder *coder)
{
	return (
		ft_gettime() - get_time(&coder->coder_mutex,
			&coder->last_compile_start) > coder->data->bornout_time
	);
}

static int	ignore(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_mutex);
	if (coder->last_compile_start == 1 || coder->finish == TRUE)
	{
		pthread_mutex_unlock(&coder->coder_mutex);
		return (TRUE);
	}
	pthread_mutex_unlock(&coder->coder_mutex);
	return (FALSE);
}

static int	is_simulation_over(t_data *data)
{
	pthread_mutex_lock(&data->data_mutex);
	if (data->compiles == data->coder_count)
	{
		pthread_mutex_unlock(&data->data_mutex);
		return (TRUE);
	}
	pthread_mutex_unlock(&data->data_mutex);
	return (FALSE);
}

void	*monitor(void *d)
{
	t_data	*data;
	int		i;

	data = (t_data *)d;
	while (!get_bool(&data->stop, &data->is_semulation_over))
	{
		i = -1;
		while (++i < data->coder_count)
		{
			if (ignore(&data->coders[i]))
				continue ;
			if (is_burned_out(&data->coders[i]))
				return (set_bool(&data->stop, &data->is_semulation_over, TRUE),
					print_burnout(&data->coders[i]), NULL);
		}
		if (is_simulation_over(data))
			return (set_bool(&data->stop,
					&data->is_semulation_over,
					TRUE), NULL);
		usleep(200);
	}
	return (NULL);
}
