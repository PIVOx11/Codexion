#include "codexion.h"

void wait_all(t_coder *coder)
{
    while(coder->data->is_ready == FALSE)
        ;
}

void *semulation(void *co)
{
    t_coder *coder = (t_coder *)co; 
    wait_all(coder);
    coder->coder_die = FALSE;
    coder->last_compile_start = coder->data->start_semulation;
    while(coder->data->is_semulation_over == FALSE)
    {
        compile(coder);
        debug(coder);
        refactol(coder);
        pthread_mutex_lock(&coder->data->use_data);
        if (coder->compile_count == coder->data->compile_req)
        {
            coder->data->compile_done++;
            pthread_mutex_unlock(&coder->data->use_data);
            break;
        }
        pthread_mutex_unlock(&coder->data->use_data);
    }
    if (coder->data->compile_done == coder->data->coder_count)
        coder->data->is_semulation_over == TRUE;
    return NULL;
}

