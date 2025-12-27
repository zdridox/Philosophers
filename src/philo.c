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
    if (get_state(philo) == SLEEPING)
    {
        set_state(philo, THINKING);
        if (check_sim_state(philo) != 1)
            return;
        print_state(philo, 0);
    }
    pthread_mutex_lock(&(philo->table->fork_mutexes)[f_index]);
    if (check_sim_state(philo) != 1)
    {
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
        return;
    }
    print_state(philo, 1);
    pthread_mutex_lock(&(philo->table->fork_mutexes)[s_index]);
    if (check_sim_state(philo) != 1)
    {
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
        return;
    }
    print_state(philo, 1);
    set_state(philo, EATING);
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
    print_state(philo, 0);
    usleep(philo->table->time_to_eat * 1000);
    pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
    pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
    set_state(philo, SLEEPING);
    if (check_sim_state(philo) != 1)
        return;
    print_state(philo, 0);
    usleep(philo->table->time_to_sleep * 1000);
}

void *philosopher(void *arg)
{
    t_philo *philo;
    int offset;
    int first_index;
    int second_index;

    philo = (t_philo *)arg;
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
