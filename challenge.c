#include <stdio.h>
#include <pthread.h>



struct threads{
    pthread_t t1;
    pthread_t t2;
    int i ;
    int current;
    pthread_mutex_t mutex; 
    pthread_cond_t v;
};



void *routine(void *data)
{
    struct threads *s = (struct threads *)data;
    for(int j = 0; j < 100; j++)
    {
        pthread_mutex_lock(&s->mutex);
        s->i++;
        pthread_mutex_unlock(&s->mutex);
    } 
}

void *routine1(void *data)
{
    struct threads *s = (struct threads *)data;
    for(int j = 0; j < 100; j++)
    {
        pthread_mutex_lock(&s->mutex);
        s->i++;
        pthread_mutex_unlock(&s->mutex);
    }
}


int main()
{
    struct threads th;
    th.i = 0; 
    
    pthread_cond_init(&th.v, NULL);
    pthread_mutex_init(&th.mutex, NULL);
    th.current = 1;
    pthread_create(&th.t1, NULL, routine, &th);
    pthread_create(&th.t2, NULL, routine1, &th);

  
    pthread_join(th.t1, NULL);
    pthread_join(th.t2, NULL);
    printf("i == %d", th.i);
}