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
        table->philos[i].forks_p = &table->forks;
        pthread_create(&table->philos[i].thread, NULL, f, &table->philos[i]);
        pthread_join(table->philos[i].thread, NULL);
        i++;
    }
}

void *test_func(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    printf("state %d fork0 %d\n", philo->state, *philo->forks_p[0]);
    *philo->forks_p[0] = *philo->forks_p[0] == 0 ? 1 : 0;
    return (NULL);
}

int main()
{
    t_table *table;

    table = malloc(sizeof(t_table));
    table_init(table, 4, test_func);
    return (0);
}