/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_stage.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 09:15:28 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/08 13:59:22 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void    *semulate_one(void *c)
{
    t_coder     *coder;
    
    coder = (t_coder *)c;
    wait_all(coder);
    set_time(&coder->coder_mutex, &coder->last_compile_start, coder->data->start_semulation);
    pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
    safe_print("hase take a dongle", coder);
    while (!get_bool(&coder->data->stop, &coder->data->is_semulation_over))
    {
        usleep(500);
    }
    pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
    return NULL;
}
static int    creat_join_coders(t_data *data, int  join)
{
    int     i;

    i = -1;
    if (data->coder_count == 1 && !join)
    {
        if(pthread_create(&data->coders[0].thread_id, NULL, semulate_one, &data->coders[0]))
            return FALSE;
        return TRUE;
    }
    if (!join)
    {
        while (++i < data->coder_count)
        {
            if (pthread_create(&data->coders[i].thread_id, NULL, semulation, &data->coders[i]))
                return FALSE;
        }
    }
    while (++i < data->coder_count)
    {
        if (pthread_join(data->coders[i].thread_id, NULL) != 0)
            return FALSE;
    }
    return TRUE;
}
void wait_all(t_coder *coder)
{
    pthread_mutex_lock(&coder->data->data_mutex);
    while(!coder->data->coders_ready)
        pthread_cond_wait(&coder->data->data_cond, &coder->data->data_mutex);
    pthread_mutex_unlock(&coder->data->data_mutex);
}

void *semulation(void *co)
{
    t_coder     *coder;

    coder = (t_coder *)co;
    // wait_all(coder); no need to wait :)
    set_time(&coder->coder_mutex, &coder->last_compile_start, coder->data->start_semulation);
    if (coder->id %2 == 0)
        usleep(100);
    while (!get_bool(&coder->data->stop, &coder->data->is_semulation_over) &&
            !get_bool(&coder->coder_mutex, &coder->finish))
    {
        compile(coder);
        debug(coder);
        refactol(coder);
    }
    return NULL;
}



int start_semulation(t_data *data)
{
    data->start_semulation = ft_gettime();
    if (!data->start_semulation)
        return FALSE;
    if (!creat_join_coders(data, FALSE) ||
        pthread_create(&data->monitor, NULL, monitor, data))
        return FALSE;
    pthread_mutex_lock(&data->data_mutex);
    data->coders_ready = TRUE;
    if (pthread_cond_broadcast(&data->data_cond))
        return (pthread_mutex_unlock(&data->data_mutex), FALSE);
    pthread_mutex_unlock(&data->data_mutex);
    if (pthread_join(data->monitor, NULL))
        return FALSE;
    if (!creat_join_coders(data, TRUE))
        return FALSE;
    return TRUE;
}
