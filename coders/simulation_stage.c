#include "codexion.h"

void wait_all(t_coder *coder)
{
    while(1)
    {
        if (get_bool(&coder->data->data_mutex, &coder->data->coders_ready))
            break;
        usleep(100);
    }
}

void *semulation(void *co)
{
    t_coder     *coder;

    coder = (t_coder *)co;
    wait_all(coder);
    pthread_mutex_lock(&coder->coder_mutex);
    coder->last_compile_start = coder->data->start_semulation;
    pthread_mutex_unlock(&coder->coder_mutex);
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
    int     i;

    i = -1;
    data->coders_ready = FALSE;
    data->is_semulation_over = FALSE;
    data->compile_done = FALSE;
    while (++i < data->coder_count)
    {
        if (pthread_create(&data->coders[i].thread_id, NULL, semulation, &data->coders[i]) != 0)
            return FALSE;
    }
    i = -1;
    pthread_create(&data->monitor, NULL, monitor, data);
    data->start_semulation = ft_gettime();
    set_bool(&data->data_mutex, &data->coders_ready, TRUE); // m3a ban wla kaydr burn out f 0 :).
    while (++i < data->coder_count)
    {
        if (pthread_join(data->coders[i].thread_id, NULL) != 0)
            return FALSE;
    }
    // pthread_join(data->monitor, NULL);
    return TRUE;
}
