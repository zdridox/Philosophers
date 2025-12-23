#include "../headers/philosophers.h"

void handle_indexes(t_philo *philo, int *offset, int *f_index, int *s_index)
{
    if (philo->index != (philo->table->philo_count - 1))
        *offset = 1;
    else
        *offset = -philo->index;
    *f_index = philo->index;
    *s_index = philo->index + *offset;
    if (*f_index > *s_index)
    {
        *s_index = philo->index;
        *f_index = philo->index + *offset;
    }
}

void philo_logic(t_philo *philo, int f_index, int s_index)
{
    if (philo->state == STATE_SLEEPING)
    {
        philo->state = STATE_THINKING;
        printf("philo %d is thinking\n", philo->index);
    }
    pthread_mutex_lock(&(philo->table->fork_mutexes)[f_index]);
    pthread_mutex_lock(&(philo->table->fork_mutexes)[s_index]);
    if ((philo->table->forks)[f_index] == FORK_FREE && (philo->table->forks)[s_index] == FORK_FREE)
    {
        (philo->table->forks)[f_index] = FORK_INUSE;
        (philo->table->forks)[s_index] = FORK_INUSE;
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
        philo->state = STATE_EATING;
        gettimeofday(&philo->eaten_at, NULL);
        printf("philo %d is eating\n", philo->index);
        usleep(philo->table->time_to_eat * 1000);
        pthread_mutex_lock(&(philo->table->fork_mutexes)[f_index]);
        pthread_mutex_lock(&(philo->table->fork_mutexes)[s_index]);
        (philo->table->forks)[s_index] = FORK_FREE;
        (philo->table->forks)[f_index] = FORK_FREE;
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
        philo->state = STATE_SLEEPING;
        printf("philo %d is sleeping\n", philo->index);
        usleep(philo->table->time_to_sleep * 1000);
    }
    else
    {
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
    }
}

void *philosopher(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    int offset;
    struct timeval now;
    int first_index;
    int second_index;

    gettimeofday(&philo->eaten_at, NULL);
    while (1)
    {
        gettimeofday(&now, NULL);
        handle_indexes(philo, &offset, &first_index, &second_index);
        if (((now.tv_sec - philo->eaten_at.tv_sec) * 1000 + (now.tv_usec - philo->eaten_at.tv_usec) / 1000) > philo->table->time_to_die)
            return (philo->state = STATE_DEAD, NULL);
        philo_logic(philo, first_index, second_index);
    }
}
