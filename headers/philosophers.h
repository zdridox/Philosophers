/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzdrodow <mzdrodow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 21:27:39 by mzdrodow          #+#    #+#             */
/*   Updated: 2026/01/31 03:08:05 by mzdrodow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum s_state
{
	THINKING,
	SLEEPING,
	EATING,
	DEAD
}					t_state;

typedef struct s_philo
{
	pthread_t		thread;
	t_state			state;
	int				index;
	int				times_eaten;
	pthread_mutex_t	eaten_at_m;
	pthread_mutex_t	times_eaten_m;
	pthread_mutex_t	state_m;
	struct timeval	eaten_at;
	struct timeval	fell_asleep_at;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	int				sim_flag;
	struct timeval	start;
	pthread_mutex_t	sim_flag_m;
	pthread_mutex_t	printf_m;
	int				philo_count;
	t_philo			*philos;
	pthread_mutex_t	*fork_mutexes;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				minimal_times_eaten;
}					t_table;

void				*philosopher(void *arg);
int					safe_p_atoi(const char *str);
int					validate_input(int argc, char **argv);
void				set_state(t_philo *philo, t_state new_state);
t_state				get_state(t_philo *philo);
void				print_state(t_philo *philo, int fork_flag);
int					check_sim_state(t_table *table);
void				philo_think(t_philo *philo);
void				philo_take_fork(t_philo *philo, int fork_index);
void				philo_eat(t_philo *philo, int f_index, int s_index);
void				philo_sleep(t_philo *philo);
void				*control_thread_f(void *arg);

#endif
