#ifndef CODEXION_H
#define CODEXION_H
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

typedef pthread_mutex_t mutex;
typedef struct s_data t_data;

typedef struct s_dongle
{
    mutex   t;
    int     dongle_id;

}   t_dongle;

typedef struct s_coder
{
    int         id;
    int         compile_count;
    int         is_req_compile; //Flage to detect if it complite required compile.
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
    int         cold_down_time;
    int         is_burn_out; //Flage to detect if a coder is burn_out.
    char    *scheduler;
    t_dongle *dongles;
    t_coder  *coders;

};



void ft_error();
void parse_arg(t_data *data, char **arg);
void set_arg(char **arg, t_data *data);
long task_time(long time);
long    ft_gettime();

#endif