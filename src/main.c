#include "../headers/philosophers.h"


// TODO: /
// MUTEX on each fork and not on the whole thread
// use usleep for sleeping / eating instead of gettimeofday
// make a control thread that will be controlling the deaths of philos etc.


// not starve
int philo_count = 5;
int time_to_eat = 200;
int time_to_sleep = 200;
int time_to_die = 800;

// starve
// int philo_count = 4;
// int time_to_eat = 410;
// int time_to_sleep = 200;
// int time_to_die = 800;

void table_init(t_table *table, int n, void(*f(void *)))
{
    int i;

    i = 0;
    table->philo_count = n;
    table->forks = malloc(sizeof(int) * n);
    table->philos = malloc(sizeof(t_philo) * n);
    pthread_mutex_init(&table->lock, NULL);
    while (i < n)
    {
        table->forks[i] = FORK_FREE;
        table->philos[i].state = STATE_THINKING;
        table->philos[i].index = i;
        table->philos[i].forks_p = &table->forks;
        table->philos[i].lock_p = &table->lock;
        table->philos[i].philo_count_p = &table->philo_count;
        pthread_create(&table->philos[i].thread, NULL, f, &table->philos[i]);
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
    int offset;
    struct timeval now;

    // philo starts with a full belly :)
    gettimeofday(&philo->eaten_at, NULL);
    while (philo->state != STATE_DEAD)
    {
        pthread_mutex_lock(philo->lock_p);
        gettimeofday(&now, NULL);
        if(philo->index != (*philo->philo_count_p - 1))
            offset = 1;
        else 
            offset = -philo->index;
        if(((now.tv_sec - philo->eaten_at.tv_sec) * 1000 + (now.tv_usec - philo->eaten_at.tv_usec) / 1000) > time_to_die) {
            philo->state = STATE_DEAD;
            printf("philo %d starved to death\n", philo->index);
            continue;
        }
        if(philo->state == STATE_SLEEPING) {
            if(((now.tv_sec - philo->fell_asleep_at.tv_sec) * 1000 + (now.tv_usec - philo->fell_asleep_at.tv_usec) / 1000) > time_to_sleep) {
                philo->state = STATE_THINKING;
                printf("philo %d is thinking\n", philo->index);
            }
        }
        if(philo->state != STATE_EATING && philo->state != STATE_SLEEPING) {
                if((*philo->forks_p)[philo->index] == FORK_FREE && (*philo->forks_p)[philo->index + offset] == FORK_FREE) {
                    (*philo->forks_p)[philo->index] = FORK_INUSE;
                    (*philo->forks_p)[philo->index + offset] = FORK_INUSE;
                    philo->state = STATE_EATING;
                    gettimeofday(&philo->eaten_at, NULL);
                    printf("philo %d is eating\n", philo->index);
                }
        }
        if (philo->state == STATE_EATING){
            if(((now.tv_sec - philo->eaten_at.tv_sec) * 1000 + (now.tv_usec - philo->eaten_at.tv_usec) / 1000) > time_to_eat) {
                    (*philo->forks_p)[philo->index] = FORK_FREE;
                    (*philo->forks_p)[philo->index + offset] = FORK_FREE;
                    gettimeofday(&philo->eaten_at, NULL);
                    gettimeofday(&philo->fell_asleep_at, NULL);
                    philo->state = STATE_SLEEPING;
                    printf("philo %d, fell asleep\n", philo->index);
            }
        }
        pthread_mutex_unlock(philo->lock_p);
    }
    return (NULL);
}

int main()
{
    t_table *table;

    table = malloc(sizeof(t_table));
    table_init(table, philo_count, test_func);
    return (0);
}
