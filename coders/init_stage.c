/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:55:54 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/05 11:18:18 by blidriss         ###   ########.fr       */
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
        if (pthread_mutex_init(&coder->coder_mutex, NULL) != 0 || 
            pthread_cond_init(&coder->coder_cond, NULL) != 0)
            return FALSE;
        coder->id = i + 1;
        coder->compile_count = 0;
        coder->right_dongle = &data->dongles[i];
        coder->left_dongle = &data->dongles[(i + 1) % data->coder_count];
        coder->data = data;
        coder->last_compile_start = 1;
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
        if ((pthread_mutex_init(&dongle->dongle_mutex, NULL)) != 0 ||
            pthread_cond_init(&dongle->dongle_cond, NULL) != 0 ||
            pthread_mutex_init(&dongle->d_data, NULL))
            break;
    }
    return TRUE;
}

int    full_init(t_data *data) // full init, the begenning point of init
{
    data->coders = malloc(sizeof(t_coder) * data->coder_count);
    data->dongles = malloc(sizeof(t_dongle) * data->coder_count);
    if (!data->coders || !data->dongles)
        return (malloc_clean(data), FALSE);
    pthread_mutex_init(&data->stop, NULL);
    pthread_mutex_init(&data->data_mutex, NULL);
    pthread_cond_init(&data->data_cond, NULL);
    if (!fill_dongles_init_mutex(data))
        return FALSE;
    if (!fill_coders(data))
        return FALSE;
    return TRUE;
}

