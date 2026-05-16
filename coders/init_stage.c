/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:55:54 by blidriss          #+#    #+#             */
/*   Updated: 2026/05/16 08:56:04 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void *test()
{
    /*
        test function to check if threads are created :)
    */
    static int i = 0;
    printf("a Tread number: %d createde :)\n", ++i);
    return NULL;
}
static void fill_coders(t_data *data)
{
    /*
        fill coders data :)
    */
    int     i;
    t_coder *coder;

    i = -1;
    while(++i < data->coder_count)
    {
        coder = data->coders + i;
        coder->id = i;
        coder->compile_count = 0;
        coder->is_compile_finish = false;
        coder->data = data;
    }
}

static void fill_dongles_init_mutex(t_data *data)
{
    int         i;
    t_dongle    *t;

    i = -1;
    while (++i < data->coder_count)
    {
        t = &data->dongles[i];
        t->dongle_id = i + 1;
        if ((pthread_mutex_init(&t->t, NULL)) != 0)
            break;
    }
    if (i < data->coder_count)
    {
        while (--i >= 0)
            pthread_mutex_destroy(&data->dongles[i].t);
        free_all(data);
    }
}
void init_threads(t_data *data)
{
    int     i;
    t_coder *coder;

    i = -1;
    coder = data->coders;
    while (++i < data->coder_count)
    {
        if(pthread_create(&coder[i].thread_id, NULL, test, NULL) != 0)
            free_all(data);
    }
    i = -1;
    while (++i < data->coder_count)
    {
        if(pthread_join(coder[i].thread_id, NULL) != 0)
            free_all(data);
    }
    
}
int    full_init(t_data *data)
{
    data->is_semulation_over = 0;
    data->coders = malloc(sizeof(t_coder) * data->coder_count);
    data->dongles = malloc(sizeof(t_dongle) * data->coder_count);
    if (!data->coders || !data->dongles)
        return (free_all(data), 0);
    fill_coders(data);
    fill_dongles_init_mutex(data);
    return 1;
}
