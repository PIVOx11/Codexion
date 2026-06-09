/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:56:07 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/09 11:41:15 by blidriss         ###   ########.fr       */
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
#include <limits.h>


#define FALSE 0
#define TRUE 1

typedef pthread_mutex_t mutex;
typedef pthread_cond_t cond;
typedef struct s_data t_data;
typedef struct s_coder t_coder;

typedef struct s_request
{
    t_coder *coder;
    long    request_time;
    long    dead_line;

}   t_request;

typedef struct s_dongle
{
    mutex   dongle_mutex;
    mutex   d_data;
    cond    dongle_cond;
    long    last_relais;
    int     is_taken;
    int     dongle_id;
    t_data *data;
    int     heap_size;
    t_request heap[2];

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
    int         finish;
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
    char        *scheduler;
    int         is_semulation_over;
    long        start_semulation;
    int         coders_ready;
    int         compiles;
    int         compile_done;
    pthread_t   monitor;
    mutex       stop;
    mutex       data_mutex;
    mutex       print;
    cond        data_cond;
    t_dongle    *dongles;
    t_coder     *coders;
};

// parsing stage
int     parse_arg(t_data *data, char **arg);
void    set_arg(char **arg, t_data *data);

// time functions
long    task_time(long time);
long    ft_gettime();

// init stage
int     full_init(t_data *data);

/*clean up functons*/
void    malloc_clean(t_data *data);
void    coder_mutex_destroy(t_coder *coder, int counter);
void    dongle_destroy(t_dongle *dongles, int counter);
void	clean_data(t_data *data, int counter);
void	clean_resources(t_data *data);

// simulation functions
int     start_semulation(t_data *data);
void    *semulation(void *co);

// Thread Actions
void    compile(t_coder *coder);
void    debug(t_coder *coder);
void    refactol(t_coder *coder);

// monitor
void    *monitor(void *d);

// seters and geters
int     get_bool(mutex *mtx, int *value);
int    set_bool(mutex *mtx, int *dest, int value);
long    get_time(mutex *mtx, long *time);
int    set_time(mutex *mtx, long *dest, long value);

// safety functions
void    safe_print(char *str, t_coder *coder);
void    safe_sleep(t_coder *coder, int time);
void    wait_all(t_coder *coder);
void print_burnout(t_coder *coder);

// scheduler
t_coder *get_winner(t_dongle *dongle);
void    mutex_lock(t_coder *coder);
void    remove_request(t_coder *coder, t_dongle *dongle);
void    add_request(t_coder *coder);
void    fill_request(t_coder *coder, t_dongle *first, t_dongle *second);
int     try_take_dongle(t_dongle *dongle, t_coder *coder);
void    wait_dongle(t_coder *coder, t_dongle *dongle);
void    dongle_order(t_coder *coder, t_dongle **f, t_dongle **s);

#endif