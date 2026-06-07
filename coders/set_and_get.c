/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_and_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 09:15:10 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/07 09:15:18 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int    get_bool(mutex *mtx, int *value)
{
    int     res;

    if (pthread_mutex_lock(mtx))
        return -1;
    res =  *value;
    if (pthread_mutex_unlock(mtx))
        return -1;
    return res;
}

int set_bool(mutex *mtx, int *dest, int value)
{
    if (pthread_mutex_lock(mtx))
        return FALSE;
    *dest = value;
    if (pthread_mutex_unlock(mtx))
        return FALSE;
    return TRUE;
}

long    get_time(mutex *mtx, long *time)
{
    long     res;

    if (pthread_mutex_lock(mtx))
        return -1;
    res =  *time;
    if (pthread_mutex_unlock(mtx))
        return -1;
    return res; 
}

int    set_time(mutex *mtx, long *dest, long value)
{
    if (pthread_mutex_lock(mtx))
        return FALSE;
    *dest = value;
    if (pthread_mutex_unlock(mtx))
        return FALSE;
    return TRUE;
}
