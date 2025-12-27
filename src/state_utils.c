#include "../headers/philosophers.h"

void set_state(t_philo *philo, t_state new_state)
{
    pthread_mutex_lock(&philo->state_m);
    philo->state = new_state;
    pthread_mutex_unlock(&philo->state_m);
}

int get_state(t_philo *philo)
{
    int res;

    pthread_mutex_lock(&philo->state_m);
    res = philo->state;
    pthread_mutex_unlock(&philo->state_m);
    return (res);
}
