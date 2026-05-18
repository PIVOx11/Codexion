#include "codexion.h"

void compile(t_coder *coder)
{
    long    t;
    if (coder->id %2 != 0)
    {    
        pthread_mutex_lock(&coder->left_dongle->t);
        t = task_time(coder->data->start_semulation);
        if (coder->data->is_semulation_over)
        return;
        printf("%ld %d  has taken a dongle\n", t, coder->id);
        pthread_mutex_lock(&coder->right_dongle->t);
        t = task_time(coder->data->start_semulation);
        printf("%ld %d  has taken a dongle\n", t, coder->id);
    }
    else
    {
        pthread_mutex_lock(&coder->right_dongle->t);
        t = task_time(coder->data->start_semulation);
        if (coder->data->is_semulation_over)
        return;
        printf("%ld %d  has taken a dongle\n", t, coder->id);
        pthread_mutex_lock(&coder->left_dongle->t);
        t = task_time(coder->data->start_semulation);
        printf("%ld %d  has taken a dongle\n", t, coder->id);    
    }
    
    pthread_mutex_lock(&coder->t);
    t = task_time(coder->data->start_semulation);
    coder->last_compile_start = ft_gettime();
    if (coder->data->is_semulation_over)
        return;
    printf("%ld %d is compiling\n", t, coder->id);
    coder->compile_count++;
    pthread_mutex_unlock(&coder->t);

    usleep(coder->data->compile_time * 1000);

    pthread_mutex_unlock(&coder->left_dongle->t);
    pthread_mutex_unlock(&coder->right_dongle->t);
}


void debug(t_coder *coder)
{
    if (coder->data->is_semulation_over)
        return;
    long t = task_time(coder->data->start_semulation);
    pthread_mutex_lock(&coder->t);
    if (coder->data->is_semulation_over)
        return;
    printf("%ld %d is debugging\n", t, coder->id);
    pthread_mutex_unlock(&coder->t);
    usleep(coder->data->debug_time * 1000);
}


void refactol(t_coder *coder)
{
    if (coder->data->is_semulation_over)
        return;
    long t = task_time(coder->data->start_semulation);
    pthread_mutex_lock(&coder->t);
    if (coder->data->is_semulation_over)
        return;
    printf("%ld %d is refactoring\n", t, coder->id);
    pthread_mutex_unlock(&coder->t);
    usleep(coder->data->refactor_time * 1000);
}
