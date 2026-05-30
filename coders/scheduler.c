#include "codexion.h"


static void    add_request(t_coder *coder, t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_mutex);
    if (dongle->heap_size >= 2)
    {
        /* heap is fixed-size (2). Drop request to avoid overflow. */
        pthread_mutex_unlock(&dongle->dongle_mutex);
        return;
    }
    dongle->heap[dongle->heap_size].coder = coder;
    dongle->heap[dongle->heap_size].request_time = ft_gettime();
    dongle->heap[dongle->heap_size].dead_line = 
        coder->last_compile_start + coder->data->bornout_time;
    dongle->heap_size++;
    pthread_mutex_unlock(&dongle->dongle_mutex);
    return;
}

static int     can_take_dongle(t_dongle *dongle, t_coder *coder)
{
    if (dongle->is_taken)
        return FALSE;
    if (get_winner(dongle) != coder)
        return FALSE;
    if (dongle->last_relais != 1 && ft_gettime() - dongle->last_relais < coder->data->cold_down_time)
        return FALSE;
    return TRUE;
}

t_coder     *get_winner(t_dongle *dongle)
{
    if (dongle->heap_size == 0)
        return NULL;
    if (dongle->heap_size == 1)
        return dongle->heap[0].coder;
    if (strcmp(dongle->data->scheduler, "fifo") == 0)
    {
        if (dongle->heap[0].request_time < dongle->heap[1].request_time)
            return dongle->heap[0].coder;
        else
            return dongle->heap[1].coder;
    }
    if (dongle->heap[0].dead_line < dongle->heap[1].dead_line)
        return dongle->heap[0].coder;
    else
        return dongle->heap[1].coder;
    return NULL;
}

void    try_to_take_dongles(t_coder *coder)
{
    add_request(coder, coder->left_dongle);
    add_request(coder, coder->right_dongle);

    while (1)
    {
        mutex_lock(coder);
        if (can_take_dongle(coder->left_dongle, coder) &&
            can_take_dongle(coder->right_dongle, coder))
                break;
        pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
        pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
        usleep(200);
    }
    // printf("OUUUT\n");
    coder->left_dongle->is_taken = TRUE;
    coder->right_dongle->is_taken = TRUE;
    safe_print("take a dongle", coder);
    safe_print("take a dongle", coder);
    remove_request(coder, coder->left_dongle);
    remove_request(coder, coder->right_dongle);
}

void    mutex_lock(t_coder *coder)
{
    if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
    {
        pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
        pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
        return;
    }
    pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
    pthread_mutex_lock(&coder->left_dongle->dongle_mutex);

}

void remove_request(t_coder *coder, t_dongle *dongle)
{
    int     i;

    i = -1;
    while (++i < dongle->heap_size)
    {
        if (dongle->heap[i].coder == coder)
        {
            dongle->heap[i] = dongle->heap[1];
            break;
        }
    }
    dongle->heap_size--;
}
