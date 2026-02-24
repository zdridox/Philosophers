/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzdrodow <mzdrodow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 21:28:57 by mzdrodow          #+#    #+#             */
/*   Updated: 2026/02/24 18:17:30 by mzdrodow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

void	handle_indexes(t_philo *philo, int *offset, int *f_index, int *s_index)
{
	if (philo->index != (philo->table->philo_count - 1))
		*offset = 1;
	else
		*offset = -philo->index;
	*f_index = philo->index;
	*s_index = philo->index + *offset;
	if (*f_index > *s_index)
	{
		*s_index = philo->index;
		*f_index = philo->index + *offset;
	}
}

void	philo_logic(t_philo *philo, int f_index, int s_index)
{
	int	fork_check;

	fork_check = 0;
	philo_think(philo);
	fork_check += philo_take_fork(philo, f_index);
	fork_check += philo_take_fork(philo, s_index);
	if (fork_check != 2)
	{
		pthread_mutex_unlock(&(philo->table->fork_mutexes)[f_index]);
		pthread_mutex_unlock(&(philo->table->fork_mutexes)[s_index]);
		return ;
	}
	philo_eat(philo, f_index, s_index);
	philo_sleep(philo);
}

void	*philosopher(void *arg)
{
	t_philo	*philo;
	int		offset;
	int		first_index;
	int		second_index;

	philo = (t_philo *)arg;
	if (philo->table->philo_count == 1)
		return (NULL);
	while (1)
	{
		pthread_mutex_lock(&philo->table->start_flag_m);
		if(philo->table->start_flag == 1)
		{
			pthread_mutex_unlock(&philo->table->start_flag_m);
			break;
		}
		pthread_mutex_unlock(&philo->table->start_flag_m);
		//usleep(100);
	}
	while (1)
	{
		if (!check_sim_state(philo->table))
			break ;
		handle_indexes(philo, &offset, &first_index, &second_index);
		philo_logic(philo, first_index, second_index);
	}
	return (NULL);
}
