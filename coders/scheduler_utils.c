/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 10:24:29 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/09 11:50:42 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void dongle_order(t_coder *coder, t_dongle **f, t_dongle **s)
{
    if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
    {
        *f = coder->left_dongle;
        *s = coder->right_dongle;
        return;
    }
    *f = coder->right_dongle;
    *s = coder->left_dongle;
}
void remove_request(t_coder *coder, t_dongle *dongle)
{
    int     i;

    i = -1;
    while (++i < dongle->heap_size)
    {
        if (dongle->heap[i].coder == coder)
        {
            dongle->heap[i] =  dongle->heap[1];
            break;
        }
    }
    dongle->heap_size--;
}

void    fill_request(t_coder *coder, t_dongle *first, t_dongle *second)
{
    pthread_mutex_lock(&first->d_data);
    if (first->heap_size < 2)
    {
        first->heap[first->heap_size].coder = coder;
        first->heap[first->heap_size].dead_line = coder->last_compile_start + coder->data->bornout_time;
        first->heap_size++;
    }
    pthread_mutex_unlock(&first->d_data);
    pthread_mutex_lock(&second->d_data);
    if (second->heap_size < 2)
    {
        second->heap[second->heap_size].coder = coder;
        second->heap[second->heap_size].dead_line = coder->last_compile_start + coder->data->bornout_time;
        second->heap_size++; 
    }
    pthread_mutex_unlock(&second->d_data);
}
