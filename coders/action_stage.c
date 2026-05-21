#include "codexion.h"

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
        safe_print("take a dongle", coder);
        pthread_mutex_lock(&second->dongle_mutex);
        safe_print("take a dongle", coder);
    }
    else
    {
        pthread_mutex_unlock(&first->dongle_mutex);
        pthread_mutex_unlock(&second->dongle_mutex);
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
