/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:55:54 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/06 11:40:03 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


static int fill_coders(t_data *data)
{
    int     i;
    t_coder *coder;

    i = -1;
    while(++i < data->coder_count)
    {
        coder = data->coders + i;
        if (pthread_mutex_init(&coder->coder_mutex, NULL))
            return (coder_mutex_destroy(data->coders, i), FALSE);
        coder->id = i + 1;
        coder->compile_count = 0;
        coder->right_dongle = &data->dongles[i];
        coder->left_dongle = &data->dongles[(i + 1) % data->coder_count];
        coder->data = data;
        coder->last_compile_start = 1;
        coder->finish = FALSE;
    }
    return TRUE;
}

static int fill_dongles_init_mutex(t_data *data) 
{
    int         i;
    t_dongle    *dongle;

    i = -1;
    while (++i < data->coder_count)
    {
        dongle = &data->dongles[i];
        dongle->dongle_id = i + 1;
        dongle->last_relais = 1;
        dongle->data = data;
        dongle->heap_size = 0;
        dongle->is_taken = FALSE;
        if (pthread_mutex_init(&dongle->dongle_mutex, NULL) ||
            pthread_mutex_init(&dongle->d_data, NULL))
            return (dongle_mutex_destroy(data->dongles, i), FALSE);
    }
    return TRUE;
}

int    full_init(t_data *data)
{
    data->coders = malloc(sizeof(t_coder) * data->coder_count);
    data->dongles = malloc(sizeof(t_dongle) * data->coder_count);
    if (!data->coders || !data->dongles)
        return (malloc_clean(data), FALSE);
        
    if (pthread_mutex_init(&data->stop, NULL))
        return (malloc_clean(data), FALSE);
    if (pthread_mutex_init(&data->data_mutex, NULL) )
        return (clean_data(data, 1), FALSE);
    if (pthread_cond_init(&data->data_cond, NULL))
        return (clean_data(data, 2), FALSE);
            
    if (!fill_dongles_init_mutex(data))
        return (clean_data(data, 3), FALSE);
    if (!fill_coders(data))
        return (clean_data(data, 3),
                dongle_mutex_destroy(data->dongles, data->coder_count),
                FALSE);
    return TRUE;
}

