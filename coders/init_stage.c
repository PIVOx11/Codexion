/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:55:54 by blidriss          #+#    #+#             */
/*   Updated: 2026/05/20 23:00:45 by blidriss         ###   ########.fr       */
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
        if (pthread_mutex_init(&coder->coder_mutex, NULL) != 0)
            break;
        coder->id = i + 1;
        coder->compile_count = 0;
        coder->right_dongle = &data->dongles[i];
        coder->left_dongle = &data->dongles[(i + 1) % data->coder_count];
        coder->data = data;
        coder->coder_die = FALSE;
        coder->last_compile_start = 1;
    }
    if (i < data->coder_count)
    {
        while (--i >= 0)
            pthread_mutex_destroy(&coder->coder_mutex);
        return FALSE;
    }
    return TRUE;
}

static int fill_dongles_init_mutex(t_data *data) 
{
    int         i;
    t_dongle    *t;

    i = -1;
    pthread_mutex_init(&data->stop, NULL);
    pthread_mutex_init(&data->data_mutex, NULL);
    
    while (++i < data->coder_count)
    {
        t = &data->dongles[i];
        t->dongle_id = i;
        t->last_relais = 1;
        if ((pthread_mutex_init(&t->dongle_mutex, NULL)) != 0)
            break;
    }
    if (i < data->coder_count)
    {
        while (--i >= 0)
            pthread_mutex_destroy(&data->dongles[i].dongle_mutex);
        return FALSE;
    }
    return TRUE;
}


int    full_init(t_data *data) // full init, the begenning point of init
{
    data->coders = malloc(sizeof(t_coder) * data->coder_count);
    data->dongles = malloc(sizeof(t_dongle) * data->coder_count);
    if (!data->coders || !data->dongles)
        return FALSE;
    if (!fill_dongles_init_mutex(data))
        return FALSE;
    if (!fill_coders(data))
        return FALSE;
    return TRUE;
}
