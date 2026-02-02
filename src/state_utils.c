/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzdrodow <mzdrodow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 21:29:07 by mzdrodow          #+#    #+#             */
/*   Updated: 2026/02/02 16:42:20 by mzdrodow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

void	set_state(t_philo *philo, t_state new_state)
{
	if (new_state == EATING)
	{
		pthread_mutex_lock(&philo->eaten_at_m);
		gettimeofday(&philo->eaten_at, NULL);
		pthread_mutex_unlock(&philo->eaten_at_m);
	}
	pthread_mutex_lock(&philo->state_m);
	philo->state = new_state;
	pthread_mutex_unlock(&philo->state_m);
}

t_state	get_state(t_philo *philo)
{
	t_state	res;

	pthread_mutex_lock(&philo->state_m);
	res = philo->state;
	pthread_mutex_unlock(&philo->state_m);
	return (res);
}
