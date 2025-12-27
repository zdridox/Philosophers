#include "../headers/philosophers.h"

void death_check(t_table *table)
{
	struct timeval now;
	int i;

	i = 0;
	gettimeofday(&now, NULL);
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&table->philos[i].eaten_at_m);
		if (((now.tv_sec - table->philos[i].eaten_at.tv_sec) * 1000 + (now.tv_usec - table->philos[i].eaten_at.tv_usec) / 1000) > table->time_to_die)
		{
			pthread_mutex_unlock(&table->philos[i].eaten_at_m);
			set_state(&table->philos[i], DEAD);
			print_state(&table->philos[i], 0);
			pthread_mutex_lock(&table->sim_flag_m);
			table->sim_flag = 0;
			pthread_mutex_unlock(&table->sim_flag_m);
			break;
		}
		else
			pthread_mutex_unlock(&table->philos[i].eaten_at_m);
		i++;
	}
}

void *control_thread_f(void *arg)
{
	t_table *table;
	int i;
	int done_philos_count;

	table = (t_table *)arg;
	i = 0;
	done_philos_count = 0;
	while (1)
	{
		pthread_mutex_lock(&table->sim_flag_m);
		if (table->sim_flag != 1)
		{
			pthread_mutex_unlock(&table->sim_flag_m);
			break;
		}
		pthread_mutex_unlock(&table->sim_flag_m);
		if (i == table->philo_count)
		{
			i = 0;
			done_philos_count = 0;
		}
		pthread_mutex_lock(&table->philos[i].times_eaten_m);
		if (table->philos[i].times_eaten >= table->minimal_times_eaten && table->minimal_times_eaten != -1)
			++done_philos_count;
		pthread_mutex_unlock(&table->philos[i].times_eaten_m);
		if (done_philos_count == table->philo_count && table->minimal_times_eaten != -1)
		{
			pthread_mutex_lock(&table->sim_flag_m);
			table->sim_flag = 0;
			pthread_mutex_unlock(&table->sim_flag_m);
		}
		death_check(table);
		++i;
	}
	return (NULL);
}

void table_init(t_table *table, int n, void *(*f)(void *))
{
	int i;

	i = 0;
	table->philo_count = n;
	table->philos = malloc(sizeof(t_philo) * n);
	table->fork_mutexes = malloc(sizeof(pthread_mutex_t) * n);
	table->sim_flag = 1;
	gettimeofday(&table->start, NULL);
	pthread_mutex_init(&table->sim_flag_m, NULL);
	while (i < n)
	{
		pthread_mutex_init(&table->fork_mutexes[i], NULL);
		i++;
	}
	i = 0;
	while (i < n)
	{
		pthread_mutex_init(&table->fork_mutexes[i], NULL);
		pthread_mutex_init(&table->philos[i].eaten_at_m, NULL);
		pthread_mutex_init(&table->philos[i].state_m, NULL);
		pthread_mutex_init(&table->philos[i].times_eaten_m, NULL);
		table->philos[i].state = THINKING;
		table->philos[i].index = i;
		table->philos[i].table = table;
		table->philos[i].times_eaten = 0;
		gettimeofday(&table->philos[i].eaten_at, NULL);
		pthread_create(&table->philos[i].thread, NULL, f, &table->philos[i]);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_table *table;
	pthread_t control_thread;
	int i;

	i = 0;
	if (argc != 5 && argc != 6)
		return (printf("invalid input"), 0);
	if (validate_input(argc, argv) != 0)
		return (printf("invalid input"), 0);
	table = malloc(sizeof(t_table));
	table->time_to_die = safe_p_atoi(argv[2]);
	table->time_to_eat = safe_p_atoi(argv[3]);
	table->time_to_sleep = safe_p_atoi(argv[4]);
	if (argc == 6)
		table->minimal_times_eaten = safe_p_atoi(argv[5]);
	else
		table->minimal_times_eaten = -1;
	table_init(table, safe_p_atoi(argv[1]), philosopher);
	pthread_create(&control_thread, NULL, control_thread_f, table);
	pthread_join(control_thread, NULL);
	while (i < table->philo_count)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
	free(table->fork_mutexes);
	free(table->philos);
	free(table);
	return (0);
}
