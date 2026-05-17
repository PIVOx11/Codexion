/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:56:17 by blidriss          #+#    #+#             */
/*   Updated: 2026/05/16 21:03:52 by blidriss         ###   ########.fr       */
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











// #include "codexion.h"

// void *semulation(void *co)
// {
//     t_coder *coder = (t_coder *)co;
//     //coder->data->start_semulation = ft_gettime();
//     while (1)
//     {
//         // exit(1);
//         compile(coder);
//         refactol(coder);
//         debug(coder);
//         pthread_mutex_lock(&coder->data->read_data);
//         int second = coder->data->compile_req;
//         int first = coder->compile_count;
//         if (first == second)
//         {   
//             printf("second %d\n", second);
//             printf("first %d\n", first);
//             pthread_mutex_unlock(&coder->data->read_data);
//             return NULL;
//         }
//         pthread_mutex_unlock(&coder->data->read_data);
//     }
//     return NULL;
// }