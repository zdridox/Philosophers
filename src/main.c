#include "../headers/philosophers.h"

void death_check(t_table *table) {
	struct timeval now;
	int i;

	i = 0;
	gettimeofday(&now, NULL);
	while (i < table->philo_count)
	{
		if (((now.tv_sec - table->philos[i].eaten_at.tv_sec) * 1000 + (now.tv_usec - table->philos[i].eaten_at.tv_usec) / 1000) > table->time_to_die) {
			table->philos[i].state = STATE_DEAD;
			printf("philo %d starved to death\n", table->philos[i].index);
			table->sim_flag = 0;
			break;
		}
		i++;
	}
}

void	*control_thread_f(void *arg)
{
	t_table	*table;
	int		i;
	int		done_philos_count;

	table = (t_table *)arg;
	i = 0;
	done_philos_count = 0;
	while (table->sim_flag == 1)
	{
		if (i == table->philo_count)
		{
			i = 0;
			done_philos_count = 0;
		}
		if (table->philos[i].times_eaten >= table->minimal_times_eaten
			&& table->minimal_times_eaten != -1)
			++done_philos_count;
		if (done_philos_count == table->philo_count
			&& table->minimal_times_eaten != -1)
			table->sim_flag = 0;
		death_check(table);
		++i;
	}
	return (NULL);
}

void	table_init(t_table *table, int n, void(*f(void *)))
{
	int	i;

	i = 0;
	table->philo_count = n;
	table->philos = malloc(sizeof(t_philo) * n);
	table->fork_mutexes = malloc(sizeof(pthread_mutex_t) * n);
	table->sim_flag = 1;
	while (i < n)
	{
		pthread_mutex_init(&table->fork_mutexes[i], NULL);
		table->philos[i].state = STATE_THINKING;
		table->philos[i].index = i;
		table->philos[i].table = table;
		gettimeofday(&table->philos[i].eaten_at, NULL);
		pthread_create(&table->philos[i].thread, NULL, f, &table->philos[i]);
		i++;
	}
	i = 0;
}

int	main(int argc, char **argv)
{
	t_table		*table;
	pthread_t	control_thread;

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
	return (0);
}
