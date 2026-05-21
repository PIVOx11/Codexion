#include "codexion.h"


static int is_burned_out(t_coder *coder)
{
    long    pass;

    pass = get_time(&coder->coder_mutex, &coder->last_compile_start);
    return (ft_gettime() - pass > coder->data->bornout_time);
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
            if (get_time(&data->coders[i].coder_mutex, &data->coders[i].last_compile_start) == 1)
                continue;
            if (get_bool(&data->data_mutex, &data->compile_done))
                return (set_bool(&data->stop, &data->is_semulation_over, 1), NULL);
            if (is_burned_out(&data->coders[i]))
                return (safe_print("is burned out", &data->coders[i]),
                    set_bool(&data->stop, &data->is_semulation_over, 1), NULL);
        }
        usleep(1000);
    }
    return NULL;
}

