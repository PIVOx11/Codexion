#include "codexion.h"


static int is_burned_out(t_coder *coder)
{
    long    pass;

    pass = get_time(&coder->coder_mutex, &coder->last_compile_start);
    return (ft_gettime() - pass > coder->data->bornout_time);
}

static int coder_status(t_coder *coder)
{
    if (pthread_mutex_lock(&coder->coder_mutex))
        return FALSE;
    if (coder->compile_count == coder->data->compile_req)
    {
        coder->finish = TRUE;
        if (pthread_mutex_lock(&coder->data->data_mutex))
            return FALSE;
        coder->data->compiles++;
        if (pthread_mutex_unlock(&coder->data->data_mutex))
            return FALSE;
    }
    if (pthread_mutex_unlock(&coder->coder_mutex))
        return FALSE;
    return TRUE;
}
static int is_simulation_over(t_data *data)
{
    int     bool;

    bool = FALSE;
    if (pthread_mutex_lock(&data->data_mutex))
        return -1;
    if (data->compiles == data->coder_count)
        bool = TRUE;
    if (pthread_mutex_unlock(&data->data_mutex))
        return -1;
    return bool;
}
void    *monitor(void *d)
{
    int     i;
    
    t_data *data = (t_data *)d;
    while (!get_bool(&data->stop, &data->is_semulation_over))
    {
        i = -1;

        while (++i < data->coder_count)
        {
            if (get_time(&data->coders[i].coder_mutex, &data->coders[i].last_compile_start) == 1 ||
                get_bool(&data->coders[i].coder_mutex, &data->coders[i].finish)
            )
                continue;
            if (!coder_status(&data->coders[i]))
                return (set_bool(&data->stop, &data->is_semulation_over, TRUE), NULL);
            if (is_simulation_over(data))
                return (set_bool(&data->stop, &data->is_semulation_over, TRUE), NULL);
            if (is_burned_out(&data->coders[i]))
                return (set_bool(&data->stop, &data->is_semulation_over,TRUE), 
                print_burnout(&data->coders[i]), NULL);
        }
        usleep(1000);
    }
    return NULL;
}

