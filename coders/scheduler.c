#include "codexion.h"


void    add_request(t_coder *coder)
{
    if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
    {
        fill_request(coder, coder->left_dongle, coder->right_dongle);
        return;
    }
    fill_request(coder, coder->right_dongle ,coder->left_dongle);
}

int     take_dongle(t_dongle *dongle, t_coder *coder)
{
    pthread_mutex_lock(&dongle->d_data);
    if (dongle->is_taken || get_winner(dongle) != coder ||
        (dongle->last_relais != 1 && ft_gettime() - dongle->last_relais < coder->data->cold_down_time)
    )
    {
        pthread_mutex_unlock(&dongle->d_data);
        return FALSE;
    }
    pthread_mutex_unlock(&dongle->d_data);
    return TRUE;
}

t_coder     *get_winner(t_dongle *dongle)
{
    if (dongle->heap_size == 0)
        return NULL;
    if (dongle->heap_size == 1)
        return dongle->heap[0].coder;
    if (strcmp(dongle->data->scheduler, "fifo") == 0)
        return dongle->heap[0].coder;
    if (dongle->heap[0].dead_line < dongle->heap[1].dead_line)
        return dongle->heap[0].coder;
    else
        return dongle->heap[1].coder;
    return NULL;
}



