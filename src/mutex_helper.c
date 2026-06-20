/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzdrodow <mzdrodow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 17:57:04 by mzdrodow          #+#    #+#             */
/*   Updated: 2026/06/20 17:58:18 by mzdrodow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

void	mutex_destroyer(t_table *table)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&table->printf_m);
	pthread_mutex_destroy(&table->sim_flag_m);
	pthread_mutex_destroy(&table->start_flag_m);
	while (i < table->philo_count)
	{
		pthread_mutex_destroy(&table->fork_mutexes[i]);
		pthread_mutex_destroy(&table->philos[i].eaten_at_m);
		pthread_mutex_destroy(&table->philos[i].times_eaten_m);
		pthread_mutex_destroy(&table->philos[i].state_m);
		++i;
	}
}

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
