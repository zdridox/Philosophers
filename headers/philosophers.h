#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>

typedef enum s_state
{
    THINKING,
    SLEEPING,
    EATING,
    DEAD
} t_state;

typedef struct s_philo
{
    pthread_t thread;
    t_state state;
    int index;
    int times_eaten;
    pthread_mutex_t eaten_at_m;
    pthread_mutex_t times_eaten_m;
    pthread_mutex_t state_m;
    struct timeval eaten_at;
    struct timeval fell_asleep_at;
    struct s_table *table;
} t_philo;

typedef struct s_table
{
    int sim_flag;
    struct timeval start;
    pthread_mutex_t sim_flag_m;
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
void set_state(t_philo *philo, t_state new_state);
t_state get_state(t_philo *philo);
void print_state(t_philo *philo, int fork_flag);

#endif
