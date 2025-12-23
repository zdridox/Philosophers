#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>

#define STATE_EATING 1
#define STATE_SLEEPING 2
#define STATE_THINKING 3
#define STATE_DEAD 4

typedef struct s_philo
{
    pthread_t thread;
    int state;
    int index;
    int times_eaten;
    struct timeval eaten_at;
    struct timeval fell_asleep_at;
    struct s_table *table;
} t_philo;

typedef struct s_table
{
    int sim_flag;
    int philo_count;
    t_philo *philos;
    pthread_mutex_t *fork_mutexes;
    int time_to_eat;
    int time_to_sleep;
    int time_to_die;
    int minimal_times_eaten;
} t_table;

void *philosopher(void *arg);
int safe_p_atoi(const char *str);
int validate_input(int argc, char **argv);

#endif
