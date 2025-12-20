#include "../headers/philosophers.h"

void table_init(t_table *table, int n, void(*f(void *)))
{
    int i;

    i = 0;
    table->philo_count = n;
    table->forks = malloc(sizeof(int) * n);
    table->philos = malloc(sizeof(t_philo) * n);
    while (i < n)
    {
        table->forks[i] = FORK_FREE;
        table->philos[i].state = STATE_THINKING;
        table->philos[i].index = i;
        table->philos[i].forks_p = &table->forks;
        pthread_create(&table->philos[i].thread, NULL, f, &table->philos[i]);
        //pthread_join(table->philos[i].thread, NULL);
        i++;
    }
    i = 0;
    while (i < n)
    {
        pthread_join(table->philos[i].thread, NULL);
        i++;
    }
}

void *test_func(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (philo->state != STATE_DEAD)
    {
        printf("index: %d\n", philo->index);
    }
    return (NULL);
}

int main()
{
    t_table *table;

    table = malloc(sizeof(t_table));
    table_init(table, 4, test_func);
    return (0);
}
