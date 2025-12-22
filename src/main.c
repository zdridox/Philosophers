#include "../headers/philosophers.h"

// add a control thread that will stop the programe when a philo is dead  / 
// and that main thread can join it and wait only for it

int philo_count = 5;
int time_to_eat = 200;
int time_to_sleep = 200;
int time_to_die = 800;

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
        table->philos[i].forks_p = &table->forks;
        table->philos[i].mutexes_p = &table->fork_mutexes;
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
    int first_index;
    int second_index;

    // philo starts with a full belly :)
    gettimeofday(&philo->eaten_at, NULL);
    while (philo->state != STATE_DEAD)
    {
        if(philo->index != (*philo->philo_count_p - 1))
            offset = 1;
        else 
            offset = -philo->index;
        first_index = philo->index;
        second_index = philo->index + offset;
        if(first_index > second_index) {
            second_index = philo->index;
            first_index = philo->index + offset;
        }
        gettimeofday(&now, NULL);
        if(((now.tv_sec - philo->eaten_at.tv_sec) * 1000 + (now.tv_usec - philo->eaten_at.tv_usec) / 1000) > time_to_die) {
            philo->state = STATE_DEAD;
            printf("philo %d starved to death\n", philo->index);
            break;
        }
        if(philo->state == STATE_SLEEPING) {
            philo->state = STATE_THINKING;
            printf("philo %d is thinking\n", philo->index);
        }
                pthread_mutex_lock(&(*philo->mutexes_p)[first_index]);
                pthread_mutex_lock(&(*philo->mutexes_p)[second_index]);
                if((*philo->forks_p)[philo->index] == FORK_FREE && (*philo->forks_p)[philo->index + offset] == FORK_FREE) {
                    (*philo->forks_p)[philo->index] = FORK_INUSE;
                    (*philo->forks_p)[philo->index + offset] = FORK_INUSE;
                    pthread_mutex_unlock(&(*philo->mutexes_p)[second_index]);
                    pthread_mutex_unlock(&(*philo->mutexes_p)[first_index]);
                    philo->state = STATE_EATING;
                    gettimeofday(&philo->eaten_at, NULL);
                    printf("philo %d is eating\n", philo->index);
                    usleep(time_to_eat * 1000);
                    pthread_mutex_lock(&(*philo->mutexes_p)[first_index]);
                    pthread_mutex_lock(&(*philo->mutexes_p)[second_index]);
                    (*philo->forks_p)[philo->index] = FORK_FREE;
                    (*philo->forks_p)[philo->index + offset] = FORK_FREE;
                    pthread_mutex_unlock(&(*philo->mutexes_p)[second_index]);
                    pthread_mutex_unlock(&(*philo->mutexes_p)[first_index]);
                    philo->state = STATE_SLEEPING;
                    printf("philo %d is sleeping\n", philo->index);
                    usleep(time_to_sleep * 1000);
                } else {
                    pthread_mutex_unlock(&(*philo->mutexes_p)[second_index]);
                    pthread_mutex_unlock(&(*philo->mutexes_p)[first_index]);
                }
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
