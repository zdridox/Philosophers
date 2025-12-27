#include "../headers/philosophers.h"

int safe_p_atoi(const char *str)
{
    int res;
    int digit;

    res = 0;
    while (*str && (*(str) >= '0' && *(str) <= '9'))
    {
        digit = *str - '0';
        if (res > (INT_MAX - digit) / 10)
            return (-1);
        res = res * 10 + digit;
        str++;
    }
    return (res);
}

void print_state(t_philo *philo, int fork_flag)
{
    struct timeval now;
    long time_diff;

    gettimeofday(&now, NULL);
    time_diff = (now.tv_sec - philo->table->start.tv_sec) * 1000L + (now.tv_usec - philo->table->start.tv_usec) / 1000L;
    if (fork_flag)
    {
        printf("%ld %d has taken a fork\n", time_diff, philo->index);
    }
    else
    {
        if (philo->state == THINKING)
            printf("%ld %d is thinking\n", time_diff, philo->index);
        if (philo->state == EATING)
            printf("%ld %d is eating\n", time_diff, philo->index);
        if (philo->state == SLEEPING)
            printf("%ld %d is sleeping\n", time_diff, philo->index);
        if (philo->state == DEAD)
            printf("%ld %d died\n", time_diff, philo->index);
    }
}

int check_sim_state(t_table *table)
{
    int res;

    pthread_mutex_lock(&table->sim_flag_m);
    res = table->sim_flag;
    pthread_mutex_unlock(&table->sim_flag_m);
    return (res);
}
