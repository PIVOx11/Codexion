#include "codexion.h"

static int    creat_join_coders(t_data *data, int  join)
{
    int     i;

    i = -1;
    if (!join)
    {
        while (++i < data->coder_count)
        {
            if (pthread_create(&data->coders[i].thread_id, NULL, semulation, &data->coders[i]))
                return FALSE;
        }
    }
    while (++i < data->coder_count)
    {
        if (pthread_join(data->coders[i].thread_id, NULL) != 0)
            return FALSE;
    }
    return TRUE;
}
void wait_all(t_coder *coder)
{
    pthread_mutex_lock(&coder->data->data_mutex);
    while(!coder->data->coders_ready)
        pthread_cond_wait(&coder->data->data_cond, &coder->data->data_mutex);
    pthread_mutex_unlock(&coder->data->data_mutex);
}

void *semulation(void *co)
{
    t_coder     *coder;

    coder = (t_coder *)co;
    wait_all(coder);
    set_time(&coder->coder_mutex, &coder->last_compile_start, coder->data->start_semulation);
    while (!get_bool(&coder->data->stop, &coder->data->is_semulation_over))
    {
        pthread_mutex_lock(&coder->data->data_mutex);
        if (coder->compile_count == coder->data->compile_req)
        {
            coder->data->compiles++;
            if (coder->data->compiles == coder->data->coder_count)
                coder->data->compile_done = TRUE;
            pthread_mutex_unlock(&coder->data->data_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&coder->data->data_mutex);
        compile(coder);
        debug(coder);
        refactol(coder);
    }
    return NULL;
}

int start_semulation(t_data *data)
{
    if (!creat_join_coders(data, FALSE) ||
        pthread_create(&data->monitor, NULL, monitor, data))
        return FALSE;
    if (pthread_mutex_lock(&data->data_mutex))
        return FALSE;
    data->start_semulation = ft_gettime();
    if (!data->start_semulation)
        return FALSE;
    data->coders_ready = TRUE;
    if (pthread_cond_broadcast(&data->data_cond))
        return FALSE;
    if (pthread_mutex_unlock(&data->data_mutex))
        return FALSE;
    if (pthread_join(data->monitor, NULL))
        return FALSE;
    if (!creat_join_coders(data, TRUE))
        return FALSE;
    return TRUE;
}
