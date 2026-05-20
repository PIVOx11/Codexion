#include "codexion.h"



void safe_print(char *str, t_coder *coder)
{
    long    time;

    pthread_mutex_lock(&coder->data->data_mutex);
    if (coder->data->is_semulation_over)
    {    
        pthread_mutex_unlock(&coder->data->data_mutex);    
        return;
    }
    time = task_time(coder->data->start_semulation);
    printf("%ld %d %s\n",
    time, coder->id, str);
    pthread_mutex_unlock(&coder->data->data_mutex);
}

void safe_sleep(t_coder *coder, int time)
{
    long    start;

    start = ft_gettime();
    while(!get_bool(&coder->data->stop, &coder->data->is_semulation_over))
    {
        if (ft_gettime() - start >= time)
            break;
        usleep(200);
    }
}