/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzdrodow <mzdrodow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 21:27:31 by mzdrodow          #+#    #+#             */
/*   Updated: 2026/01/31 03:11:32 by mzdrodow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

void	init_fork_mutexes(t_table *table, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_init(&table->fork_mutexes[i], NULL);
		i++;
	}
}

void	prepare_philos(t_table *table, void *(*f)(void *))
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
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

t_table	*table_init(int n, void *(*f)(void *))
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	table->philos = malloc(sizeof(t_philo) * n);
	table->fork_mutexes = malloc(sizeof(pthread_mutex_t) * n);
	if (!table->philos || !table->fork_mutexes)
		return (NULL);
	table->philo_count = n;
	table->sim_flag = 1;
	gettimeofday(&table->start, NULL);
	pthread_mutex_init(&table->sim_flag_m, NULL);
	pthread_mutex_init(&table->printf_m, NULL);
	init_fork_mutexes(table, n);
	prepare_philos(table, f);
	return (table);
}

void	set_input_vars(t_table *table, int argc, char **argv)
{
	table->time_to_die = safe_p_atoi(argv[2]);
	table->time_to_eat = safe_p_atoi(argv[3]);
	table->time_to_sleep = safe_p_atoi(argv[4]);
	if (argc == 6)
		table->minimal_times_eaten = safe_p_atoi(argv[5]);
	else
		table->minimal_times_eaten = -1;
}

int	main(int argc, char **argv)
{
	t_table		*table;
	pthread_t	control_thread;
	int			i;

	i = 0;
	if (argc != 5 && argc != 6)
		return (printf("invalid input"), 0);
	if (validate_input(argc, argv) != 0)
		return (printf("invalid input"), 0);
	table = table_init(safe_p_atoi(argv[1]), philosopher);
	if (table != NULL)
	{
		set_input_vars(table, argc, argv);
		pthread_create(&control_thread, NULL, control_thread_f, table);
		pthread_join(control_thread, NULL);
		while (i < table->philo_count)
		{
			pthread_join(table->philos[i].thread, NULL);
			i++;
		}
	}
	free(table->fork_mutexes);
	free(table->philos);
	free(table);
	return (0);
}
