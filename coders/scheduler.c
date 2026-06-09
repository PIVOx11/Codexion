/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:36:29 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/09 14:48:53 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#define US_PER_MS 1000
#define MIN_SLEEP_US 100
#define MAX_SLEEP_CHUNK_MS 10

void    wait_dongle(t_coder *coder, t_dongle *dongle)
{
    long    elapsed;
    long    remaining;
    long    slice;

    pthread_mutex_lock(&dongle->d_data);
    while (!try_take_dongle(dongle, coder))
        pthread_cond_wait(&dongle->dongle_cond, &dongle->d_data);
    while (!get_bool(&coder->data->stop, &coder->data->is_semulation_over))
    {
        elapsed = ft_gettime() - dongle->last_relais;
        if (elapsed >= coder->data->cold_down_time)
            break;
        remaining = coder->data->cold_down_time - elapsed;
        if (remaining > MAX_SLEEP_CHUNK_MS)
            slice = US_PER_MS;
        else
            slice = remaining * US_PER_MS;
        if (slice < MIN_SLEEP_US)
            slice = MIN_SLEEP_US;
        usleep(slice);
    }
    if (get_bool(&coder->data->stop, &coder->data->is_semulation_over))
    {
        pthread_mutex_unlock(&dongle->d_data);
        return;
    }
    dongle->is_taken = TRUE;
    pthread_mutex_unlock(&dongle->d_data);
    safe_print("has take a dongle", coder);
}

void    add_request(t_coder *coder)
{
    if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
    {
        fill_request(coder, coder->left_dongle, coder->right_dongle);
        return;
    }
    fill_request(coder, coder->right_dongle ,coder->left_dongle);
}

int     try_take_dongle(t_dongle *dongle, t_coder *coder)
{
    if (dongle->is_taken || get_winner(dongle) != coder)
        return FALSE;
    return TRUE;
}

t_coder     *get_winner(t_dongle *dongle)
{
    if (dongle->heap_size == 1)
        return dongle->heap[0].coder;
    if (dongle->data->scheduler == EDF)
    {
        if (dongle->heap[0].dead_line < dongle->heap[1].dead_line)
            return dongle->heap[0].coder;
        else
            return dongle->heap[1].coder;
    }
        return dongle->heap[0].coder;
}
