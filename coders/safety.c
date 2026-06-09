/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safety.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 10:39:20 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/09 09:21:15 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void print_burnout(t_coder *coder)
{
    long    time;

    pthread_mutex_lock(&coder->data->data_mutex);
    time = task_time(coder->data->start_semulation);
    printf("%ld %d burned out\n",
    time, coder->id);
    pthread_mutex_unlock(&coder->data->data_mutex);
}

void safe_print(char *str, t_coder *coder)
{
    long    time;

    pthread_mutex_lock(&coder->data->data_mutex);
    if (get_bool(&coder->data->stop, &coder->data->is_semulation_over))
    {    
        pthread_mutex_unlock(&coder->data->data_mutex);    
        return;
    }
    time = task_time(coder->data->start_semulation);
    printf("%ld %d %s\n",
    time, coder->id, str);
    pthread_mutex_unlock(&coder->data->data_mutex);
}

void safe_sleep(t_coder *coder, int time)
{
    long    start;

    start = ft_gettime();
    while(!get_bool(&coder->data->stop, &coder->data->is_semulation_over))
    {
        if (ft_gettime() - start >= time)
            break;
        usleep(500);
    }
}
long    ft_gettime()
{
    struct timeval t;
    
    gettimeofday(&t, NULL);
    return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}


long task_time(long time)
{
    return (ft_gettime() - time);
}
