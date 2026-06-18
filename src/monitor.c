/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 21:12:48 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/18 12:24:05 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_burned_out(t_coder *coder)
{
	return (
		ft_gettime() - get_time(&coder->coder_mutex,
			&coder->compile_start_t) > coder->data->bornout_t
	);
}

static int	ignore(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_mutex);
	if (coder->compile_start_t == 1 || coder->finish == TRUE)
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
	if (data->compiles == data->coder_c)
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
	while (!get_bool(&data->stop, &data->semulation_over))
	{
		i = -1;
		while (++i < data->coder_c)
		{
			if (ignore(&data->coders[i]))
				continue ;
			if (is_burned_out(&data->coders[i]))
				return (set_bool(&data->stop, &data->semulation_over, TRUE),
					print_burnout(&data->coders[i]), NULL);
		}
		if (is_simulation_over(data))
			return (NULL);
		usleep(50);
	}
	return (NULL);
}
