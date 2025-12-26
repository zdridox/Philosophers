#include "../headers/philosophers.h"

int check_sim_state(t_philo *philo)
{
    int res;

    pthread_mutex_lock(&philo->table->sim_flag_m);
    res = philo->table->sim_flag;
    pthread_mutex_unlock(&philo->table->sim_flag_m);
    return (res);
}

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
    if (get_state(philo) == STATE_SLEEPING)
    {
        set_state(philo, STATE_THINKING);
        if (check_sim_state(philo) != 1)
            return;
        printf("philo %d is thinking\n", philo->index);
    }
    pthread_mutex_lock(&(philo->table->fork_mutexes)[f_index]);
    pthread_mutex_lock(&(philo->table->fork_mutexes)[s_index]);
    set_state(philo, STATE_EATING);
    pthread_mutex_lock(&philo->times_eaten_m);
    ++philo->times_eaten;
    pthread_mutex_unlock(&philo->times_eaten_m);
    pthread_mutex_lock(&philo->eaten_at_m);
    gettimeofday(&philo->eaten_at, NULL);
    pthread_mutex_unlock(&philo->eaten_at_m);
    if (check_sim_state(philo) != 1)
    {
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
        return;
    }
    printf("philo %d is eating\n", philo->index);
    usleep(philo->table->time_to_eat * 1000);
    pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
    pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
    set_state(philo, STATE_SLEEPING);
    if (check_sim_state(philo) != 1)
        return;
    printf("philo %d is sleeping\n", philo->index);
    usleep(philo->table->time_to_sleep * 1000);
}

void *philosopher(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    int offset;
    int first_index;
    int second_index;

    if (philo->table->philo_count == 1)
        return (NULL);

    while (1)
    {
        pthread_mutex_lock(&philo->table->sim_flag_m);
        if (philo->table->sim_flag != 1)
        {
            pthread_mutex_unlock(&philo->table->sim_flag_m);
            break;
        }
        pthread_mutex_unlock(&philo->table->sim_flag_m);

        handle_indexes(philo, &offset, &first_index, &second_index);
        philo_logic(philo, first_index, second_index);
    }
    return (NULL);
}
