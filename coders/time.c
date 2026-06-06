/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:56:17 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/06 12:01:03 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long    ft_gettime()
{
    struct timeval t;
    if (gettimeofday(&t, NULL) == -1)
        return 0;
    return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}


long task_time(long time)
{
    long    current_time;

    current_time = ft_gettime();
    if (!current_time)
        return 0;
    return (current_time - time);
}