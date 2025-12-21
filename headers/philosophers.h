#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>

#define STATE_EATING 1
#define STATE_SLEEPING 2
#define STATE_THINKING 3
#define STATE_DEAD 4

#define FORK_FREE 0
#define FORK_INUSE 1

typedef struct s_philo
{
    pthread_t thread;
    int state;
    int index;
    struct timeval eaten_at;
    struct timeval fell_asleep_at;
    int **forks_p;
    pthread_mutex_t *lock_p;
    int *philo_count_p;
} t_philo;

typedef struct s_table
{
    pthread_mutex_t lock;
    int philo_count;
    t_philo *philos;
    int *forks;
} t_table;

#endif
