#include "codexion.h"


void    handle_cold_down(t_dongle *dongle, t_coder *coder)
{
    if (dongle->last_relais == 1)
        return;
    while (1)
    {
        pthread_mutex_lock(&dongle->cold_down);    
        if (ft_gettime() >= dongle->last_relais || get_bool(&coder->data->stop, &coder->data->is_semulation_over))
            {
                pthread_mutex_unlock(&dongle->cold_down);
                return;
            }
        pthread_mutex_unlock(&dongle->cold_down);
        usleep(200);
    }
}
static void handle_dongles(t_coder *coder, int lock)
{
    t_dongle *first;
    t_dongle *second;

    if (coder->id % 2 == 0)
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    else
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }
    if (lock)
    {
        pthread_mutex_lock(&first->dongle_mutex);
        handle_cold_down(first, coder);
        safe_print("take a dongle", coder);
        pthread_mutex_lock(&second->dongle_mutex);
        handle_cold_down(second, coder);
        safe_print("take a dongle", coder);
    }
    else
    {
        pthread_mutex_unlock(&first->dongle_mutex);
        set_time(&first->cold_down, &first->last_relais, ft_gettime() + coder->data->cold_down_time);
        pthread_mutex_unlock(&second->dongle_mutex);
        set_time(&first->cold_down, &first->last_relais, ft_gettime() + coder->data->cold_down_time);
    }
}

void compile(t_coder *coder)
{
    handle_dongles(coder, 1);
    if (get_bool(&coder->data->stop, &coder->data->is_semulation_over))
    {
        handle_dongles(coder, 0);
        return;
    }
    pthread_mutex_lock(&coder->coder_mutex);
    coder->last_compile_start = ft_gettime();
    safe_print("is compiling", coder);
    coder->compile_count++;
    pthread_mutex_unlock(&coder->coder_mutex);
    safe_sleep(coder, coder->data->compile_time);
    handle_dongles(coder, 0);
}

void debug(t_coder *coder)
{
    if (get_bool(&coder->data->stop, &coder->data->is_semulation_over))
        return;
    safe_print("is debugging", coder);
    safe_sleep(coder, coder->data->debug_time);
}


void refactol(t_coder *coder)
{
    if (get_bool(&coder->data->stop, &coder->data->is_semulation_over))
        return;
    safe_print("is refactoring", coder);
    safe_sleep(coder, coder->data->refactor_time);
}
