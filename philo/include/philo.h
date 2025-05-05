/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:19:13 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/04/29 17:47:01 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <limits.h>
# include <stdbool.h>

# define MAX_PHILOSOPHERS 200
# define MIN_PHILOSOPHERS 1

typedef struct s_philosopher
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				eat_count;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork_mutex;
	pthread_mutex_t	*right_fork_mutex;
	struct s_table	*table;
	bool			has_left_fork;
	bool			has_right_fork;
}	t_philosopher;

typedef struct s_table
{
	int				num_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_to_think;
	long			total_meals;
	long			start_time;
	bool			is_dead;
	bool			all_ate;
	pthread_t		monitor_thread;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
}	t_table;

// init.c
void	init_all(t_table *table);
void	init_table(t_table *table, char **argv);
void	init_mutex(t_table *table);
void	init_philosophers(void *philosopher);

// main.c
void	unlock_forks(t_philosopher *philo);
void	*handle_case_one(t_philosopher *philo);

// clean.c
void	clean_mutex(t_table *table);

// routine.c
void	*monitor(void *table_philos);
bool	everyone_ate(t_table *table);
bool	keep_running(t_table *table);
void	*routine(void *philosopher);
bool	philo_is_dead(t_table *table);

// states.c
void	eating(t_philosopher *philo);
void	sleeping(t_philosopher *philo);
void	thinking(t_philosopher *philo);
bool	get_forks(t_philosopher *philo);
bool	take_fork(t_table *table, pthread_mutex_t *fork);

// utils.c
long	now_ms(void);
void	print_messages(t_table *table, int id, char *msg);
void	*ft_calloc(size_t count, size_t size);
long	ft_atol(const char *str);
int		ft_atoi(const char *nptr);

#endif /* PHILO_H */