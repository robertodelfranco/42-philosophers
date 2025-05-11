/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   states.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:06:05 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/04/29 18:09:07 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	eating(t_philosopher *philo)
{
	t_table	*table;
	long	start_time;

	table = philo->table;
	start_time = now_ms();
	pthread_mutex_lock(&table->death_mutex);
	philo->last_meal_time = start_time;
	philo->eat_count++;
	pthread_mutex_unlock(&table->death_mutex);
	if (!keep_running(table))
		return ;
	print_messages(table, philo->id, "is eating");
	start_time = now_ms();
	while (keep_running(table) && now_ms() - start_time < table->time_to_eat)
		usleep(200);
	pthread_mutex_lock(&table->death_mutex);
	philo->last_meal_time = start_time;
	pthread_mutex_unlock(&table->death_mutex);
}

void	sleeping(t_philosopher *philo)
{
	t_table	*table;
	long	start_time;

	table = philo->table;
	if (!keep_running(table))
		return ;
	print_messages(table, philo->id, "is sleeping");
	start_time = now_ms();
	while (keep_running(table) && now_ms() - start_time < table->time_to_sleep)
		usleep(200);
}

void	thinking(t_philosopher *philo)
{
	t_table	*table;
	long	start_time;

	table = philo->table;
	if (!keep_running(table))
		return ;
	print_messages(table, philo->id, "is thinking");
	start_time = now_ms();
	while (keep_running(table) && now_ms() - start_time < table->time_to_think)
		usleep(200);
}

bool	get_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		if (!take_fork(philo->table, &philo->table->forks[philo->left_fork]))
			return (false);
		philo->has_left_fork = true;
		print_messages(philo->table, philo->id, "has taken a fork");
		if (!take_fork(philo->table, &philo->table->forks[philo->right_fork]))
			return (false);
		philo->has_right_fork = true;
		print_messages(philo->table, philo->id, "has taken a fork");
	}
	else
	{
		if (!take_fork(philo->table, &philo->table->forks[philo->right_fork]))
			return (false);
		philo->has_right_fork = true;
		print_messages(philo->table, philo->id, "has taken a fork");
		if (!take_fork(philo->table, &philo->table->forks[philo->left_fork]))
			return (false);
		philo->has_left_fork = true;
		print_messages(philo->table, philo->id, "has taken a fork");
	}
	return (true);
}

bool	take_fork(t_table *table, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	if (!keep_running(table))
	{
		pthread_mutex_unlock(fork);
		return (false);
	}
	return (true);
}
