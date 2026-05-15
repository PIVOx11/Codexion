#include "codexion.h"

long    ft_gettime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}


long task_time(long time)
{
    return (ft_gettime() - time);
}