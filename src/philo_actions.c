/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzdrodow <mzdrodow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 21:28:45 by mzdrodow          #+#    #+#             */
/*   Updated: 2026/02/02 17:17:55 by mzdrodow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

void	philo_eat(t_philo *philo, int f_index, int s_index)
{
	pthread_mutex_lock(&philo->times_eaten_m);
	++philo->times_eaten;
	pthread_mutex_unlock(&philo->times_eaten_m);
	set_state(philo, EATING);
	if (check_sim_state(philo->table) != 1)
	{
		pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
		pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
		return ;
	}
	print_state(philo, 0);
	usleep(philo->table->time_to_eat * 1000);
	pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
	pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
}

void	philo_sleep(t_philo *philo)
{
	set_state(philo, SLEEPING);
	if (check_sim_state(philo->table) != 1)
		return ;
	print_state(philo, 0);
	usleep(philo->table->time_to_sleep * 1000);
}

void	philo_think(t_philo *philo)
{
	if (get_state(philo) == SLEEPING)
	{
		set_state(philo, THINKING);
		if (check_sim_state(philo->table) != 1)
			return ;
		print_state(philo, 0);
	}
	if (philo->table->philo_count % 2 == 1)
	{
		if (philo->index % 2 == 0)
			usleep(philo->table->time_to_eat * 500);
		else
			usleep(philo->table->time_to_eat * 100);
	}
}

int	philo_take_fork(t_philo *philo, int fork_index)
{
	pthread_mutex_lock(&(philo->table->fork_mutexes)[fork_index]);
	if (check_sim_state(philo->table) != 1)
	{
		pthread_mutex_unlock(&(philo->table->fork_mutexes)[fork_index]);
		return (0);
	}
	print_state(philo, 1);
	return (1);
}
