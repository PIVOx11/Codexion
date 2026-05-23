/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:56:17 by blidriss          #+#    #+#             */
/*   Updated: 2026/05/23 10:44:00 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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