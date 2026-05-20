#include "codexion.h"

static void even_coders(t_coder *coder, int flag)
{
    if (flag)
    {
        pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
        safe_print("take a dongle", coder);
        pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
        safe_print("take a dongle", coder);
    }
    else
    {       
        pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
        pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
    }
}
static void odd_coders(t_coder *coder, int flag)
{
    if (flag)
    {
        pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
        safe_print("take a dongle", coder);
        pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
        safe_print("take a dongle", coder);
    }
    else
    {
        pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
        pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
    }
}

void compile(t_coder *coder)
{
    if (coder -> id % 2 == 0)
        even_coders(coder, 1);
    else
        odd_coders(coder, 1);
    if (get_bool(&coder->data->stop, &coder->data->is_semulation_over))
    {
        if (coder -> id % 2 == 0)
            even_coders(coder, 0);
        else
            odd_coders(coder, 0);
        return;
    }
    pthread_mutex_lock(&coder->coder_mutex);
    coder->last_compile_start = ft_gettime();
    safe_print("is compiling", coder);
    coder->compile_count++;
    pthread_mutex_unlock(&coder->coder_mutex);
    safe_sleep(coder, coder->data->compile_time);
    if (coder -> id % 2 == 0)
        even_coders(coder, 0);
    else
        odd_coders(coder, 0);
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
