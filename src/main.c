#include "../headers/philosophers.h"

void *control_thread_f(void *arg)
{
    t_table *table;
    int i;

    table = (t_table *)arg;
    i = 0;
    while (1)
    {
        if (i == table->philo_count)
            i = 0;
        if (table->philos[i].state == STATE_DEAD)
        {
            printf("philo %d starved to death\n", table->philos[i].index);
            break;
        }
        ++i;
    }
    return (NULL);
}

void table_init(t_table *table, int n, void(*f(void *)))
{
    int i;

    i = 0;
    table->philo_count = n;
    table->forks = malloc(sizeof(int) * n);
    table->philos = malloc(sizeof(t_philo) * n);
    table->fork_mutexes = malloc(sizeof(pthread_mutex_t) * n);
    while (i < n)
    {
        pthread_mutex_init(&table->fork_mutexes[i], NULL);
        table->forks[i] = FORK_FREE;
        table->philos[i].state = STATE_THINKING;
        table->philos[i].index = i;
        table->philos[i].table = table;
        pthread_create(&table->philos[i].thread, NULL, f, &table->philos[i]);
        i++;
    }
    i = 0;
}

int main(int argc, char **argv)
{
    t_table *table;
    pthread_t control_thread;

    if (argc != 5)
        return (0);
    table = malloc(sizeof(t_table));
    table->time_to_die = ft_atoi(argv[2]);
    table->time_to_eat = ft_atoi(argv[3]);
    table->time_to_sleep = ft_atoi(argv[4]);
    table_init(table, ft_atoi(argv[1]), philosopher);
    pthread_create(&control_thread, NULL, control_thread_f, table);
    pthread_join(control_thread, NULL);
    return (0);
}
