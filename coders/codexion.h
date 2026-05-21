/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:56:07 by blidriss          #+#    #+#             */
/*   Updated: 2026/05/20 21:20:00 by blidriss         ###   ########.fr       */
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

#define FALSE 0
#define TRUE 1

typedef pthread_mutex_t mutex;
typedef struct s_data t_data;

typedef struct s_dongle
{
    mutex   dongle_mutex;
    long    last_relais;
    int     dongle_id;

}   t_dongle;

typedef struct s_coder
{
    int         id;
    int         compile_count;
    long        last_compile_start;
    t_dongle    *left_dongle;
    t_dongle    *right_dongle;
    pthread_t   thread_id;
    mutex       coder_mutex;
    int         coder_die;
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
    int         cold_down_time;
    char    *scheduler;
    int         is_semulation_over;
    long    start_semulation;
    int     coders_ready;
    int     compiles;
    int     compile_done; 
    pthread_t monitor;
    mutex stop;
    mutex data_mutex;
    mutex print;
    t_dongle *dongles;
    t_coder  *coders;
};


int     parse_arg(t_data *data, char **arg);
void    set_arg(char **arg, t_data *data);
long    task_time(long time);
long    ft_gettime();
int     full_init(t_data *data);
void    free_all(t_data *data);
int     start_semulation(t_data *data);
void    *semulation(void *co);
void    compile(t_coder *coder);
void    debug(t_coder *coder);
void    refactol(t_coder *coder);
void    *monitor(void *d);
int     get_bool(mutex *mtx, int *value);
void    set_bool(mutex *mtx, int *dest, int value);
long    get_time(mutex *mtx, long *time);
void    safe_print(char *str, t_coder *coder);
void    safe_sleep(t_coder *coder, int time);
void    wait_all(t_coder *coder);

#endif