#include "codexion.h"

int    get_bool(mutex *mtx, int *bool)
{
    int     res;

    pthread_mutex_lock(mtx);
    res =  *bool;
    pthread_mutex_unlock(mtx);
    return res; 
}
void set_bool(mutex *mtx, int *dest, int bool)
{
    pthread_mutex_lock(mtx);
    *dest = bool;
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