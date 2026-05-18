#include "codexion.h"


void *monitor(void *d)
{
    int     i;
    t_data *data = (t_data *)d;
    while(data->is_ready == FALSE)
    ;
    usleep(1 * 1000);
    while (data->is_semulation_over == FALSE)
    {
        i = 0;
        while (i < data->coder_count)
        {
            pthread_mutex_lock(&data->coders[i].t);
            if (task_time(data->coders[i].last_compile_start) > data->bornout_time)
            {
                data->is_semulation_over = TRUE;
                data->coders[i].coder_die = TRUE;
                printf("%ld %d is burned out\n",
                    task_time(data->start_semulation), i+1);
                    pthread_mutex_unlock(&data->coders[i].t);
                    return NULL;
            }
            pthread_mutex_unlock(&data->coders[i].t);
            i++;
        }
    }
    return NULL;
}
