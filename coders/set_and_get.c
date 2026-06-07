/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_and_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 09:15:10 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/07 11:47:27 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int    get_bool(mutex *mtx, int *value)
{
    int     res;

    pthread_mutex_lock(mtx);
    res =  *value;
    pthread_mutex_unlock(mtx);
    return res;
}

int set_bool(mutex *mtx, int *dest, int value)
{
    pthread_mutex_lock(mtx);
        
    *dest = value;
    pthread_mutex_unlock(mtx);
        
    return TRUE;
}

long    get_time(mutex *mtx, long *time)
{
    long     res;

    pthread_mutex_lock(mtx);
    res =  *time;
    pthread_mutex_unlock(mtx);
    return res; 
}

int    set_time(mutex *mtx, long *dest, long value)
{
    pthread_mutex_lock(mtx);
    *dest = value;
    pthread_mutex_unlock(mtx);
    return TRUE;
}
