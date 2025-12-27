#include "../headers/philosophers.h"

void philo_eat(t_philo *philo, int f_index, int s_index)
{
    set_state(philo, EATING);
    pthread_mutex_lock(&philo->times_eaten_m);
    ++philo->times_eaten;
    pthread_mutex_unlock(&philo->times_eaten_m);
    pthread_mutex_lock(&philo->eaten_at_m);
    gettimeofday(&philo->eaten_at, NULL);
    pthread_mutex_unlock(&philo->eaten_at_m);
    if (check_sim_state(philo->table) != 1)
    {
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
        return;
    }
    print_state(philo, 0);
    usleep(philo->table->time_to_eat * 1000);
    pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
    pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
}

void philo_sleep(t_philo *philo)
{
    set_state(philo, SLEEPING);
    if (check_sim_state(philo->table) != 1)
        return;
    print_state(philo, 0);
    usleep(philo->table->time_to_sleep * 1000);
}

void philo_think(t_philo *philo)
{
    if (get_state(philo) == SLEEPING)
    {
        set_state(philo, THINKING);
        if (check_sim_state(philo->table) != 1)
            return;
        print_state(philo, 0);
    }
}

void philo_take_fork(t_philo *philo, int fork_index)
{
    pthread_mutex_lock(&(philo->table->fork_mutexes)[fork_index]);
    if (check_sim_state(philo->table) != 1)
    {
        pthread_mutex_unlock(&(philo->table->fork_mutexes)[fork_index]);
        return;
    }
    print_state(philo, 1);
}
