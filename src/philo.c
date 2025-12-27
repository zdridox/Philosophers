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
    philo_think(philo);
    philo_take_fork(philo, f_index);
    philo_take_fork(philo, s_index);
    philo_eat(philo, f_index, s_index);
    philo_sleep(philo);
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
        if (!check_sim_state(philo->table))
            break;
        handle_indexes(philo, &offset, &first_index, &second_index);
        philo_logic(philo, first_index, second_index);
    }
    return (NULL);
}
