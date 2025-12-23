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
    philo->state = STATE_EATING;
    ++philo->times_eaten;
    gettimeofday(&philo->eaten_at, NULL);
    printf("philo %d is eating\n", philo->index);
    usleep(philo->table->time_to_eat * 1000);
    pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
    pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
    philo->state = STATE_SLEEPING;
    printf("philo %d is sleeping\n", philo->index);
    usleep(philo->table->time_to_sleep * 1000);
}

void *philosopher(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    int offset;
    int first_index;
    int second_index;

    while (philo->state != STATE_DEAD && philo->table->sim_flag == 1)
    {
        handle_indexes(philo, &offset, &first_index, &second_index);
        philo_logic(philo, first_index, second_index);
    }
    return (NULL);
}
