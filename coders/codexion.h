/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:56:07 by blidriss          #+#    #+#             */
/*   Updated: 2026/05/16 08:55:52 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define false 0
#define true 1

typedef pthread_mutex_t mutex;
typedef struct s_data t_data;

typedef struct s_dongle
{
    mutex   t;
    long    last_relais;
    int     dongle_id;

}   t_dongle;

typedef struct s_coder
{
    int         id;
    int         compile_count;
    int         is_compile_finish; //Flage to detect if it complite required compile.
    long        last_time_compile;
    t_dongle    *left_dongle;
    t_dongle    *left_right;
    pthread_t   thread_id;
    t_data      *data;

}   t_coder;

struct s_data
{
    int         coder_count;
    int         bornout_time;
    int         compile_time;
    int         debug_time;
    int         refactor_time;
    int         compile_req;
    int         is_semulation_over;
    int         cold_down_time;
    int         is_burn_out; //Flage to detect if a coder is burn_out.
    char    *scheduler;
    t_dongle *dongles;
    t_coder  *coders;
};


void   ft_error();
void   parse_arg(t_data *data, char **arg);
void   set_arg(char **arg, t_data *data);
long   task_time(long time);
long   ft_gettime();
int    full_init(t_data *data);
void   free_all(t_data *data);
void init_threads(t_data *data);

#endif