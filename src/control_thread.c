/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzdrodow <mzdrodow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 21:28:29 by mzdrodow          #+#    #+#             */
/*   Updated: 2026/02/03 00:20:07 by mzdrodow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

void	death_check(t_table *table)
{
	struct timeval	now;
	int				i;

	i = 0;
	gettimeofday(&now, NULL);
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&table->philos[i].eaten_at_m);
		if (((now.tv_sec - table->philos[i].eaten_at.tv_sec) * 1000
				+ (now.tv_usec - table->philos[i].eaten_at.tv_usec)
				/ 1000) > table->time_to_die)
		{
			pthread_mutex_unlock(&table->philos[i].eaten_at_m);
			pthread_mutex_lock(&table->sim_flag_m);
			table->sim_flag = 0;
			pthread_mutex_unlock(&table->sim_flag_m);
			set_state(&table->philos[i], DEAD);
			print_state(&table->philos[i], 0);
			break ;
		}
		else
			pthread_mutex_unlock(&table->philos[i].eaten_at_m);
		i++;
	}
}

int	check_eaten_times(t_table *table)
{
	int	i;
	int	done_philos_count;

	i = 0;
	done_philos_count = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&table->philos[i].times_eaten_m);
		if (table->philos[i].times_eaten >= table->minimal_times_eaten)
			++done_philos_count;
		pthread_mutex_unlock(&table->philos[i].times_eaten_m);
		if (done_philos_count == table->philo_count)
			return (1);
	}
	return (0);
}

void	*control_thread_f(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		if (!check_sim_state(table))
			break ;
		if (table->minimal_times_eaten != -1 && check_eaten_times(table))
		{
			pthread_mutex_lock(&table->sim_flag_m);
			table->sim_flag = 0;
			pthread_mutex_unlock(&table->sim_flag_m);
		}
		death_check(table);
	}
	return (NULL);
}
