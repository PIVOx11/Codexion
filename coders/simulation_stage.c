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
    while (1)
    {
        compile(coder);
        debug(coder);
        refactol(coder);
        pthread_mutex_lock(&coder->data->read_data);
        if (coder->compile_count == coder->data->compile_req)
        {   
            pthread_mutex_unlock(&coder->data->read_data);
            break;
        }
        pthread_mutex_unlock(&coder->data->read_data);
    }
    return NULL;
}

