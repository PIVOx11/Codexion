#include "codexion.h"

int    get_bool(mutex *mtx, int *value)
{
    int     res;

    pthread_mutex_lock(mtx);
    res =  *value;
    pthread_mutex_unlock(mtx);
    return res; 
}

void set_bool(mutex *mtx, int *dest, int value)
{
    pthread_mutex_lock(mtx);
    *dest = value;
    pthread_mutex_unlock(mtx);
}

long    get_time(mutex *mtx, long *time)
{
    long     res;

    pthread_mutex_lock(mtx);
    res =  *time;
    pthread_mutex_unlock(mtx);
    return res; 
}

void    set_time(mutex *mtx, long *dest, long value)
{
    pthread_mutex_lock(mtx);
    *dest = value;
    pthread_mutex_unlock(mtx);
}