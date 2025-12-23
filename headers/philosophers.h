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
    struct s_table *table;
} t_philo;

typedef struct s_table
{
    int philo_count;
    t_philo *philos;
    int *forks;
    pthread_mutex_t *fork_mutexes;
    int time_to_eat;
    int time_to_sleep;
    int time_to_die;
} t_table;

void *philosopher(void *arg);
int ft_atoi(const char *str);

#endif
