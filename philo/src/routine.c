/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:10:49 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/04/29 18:01:13 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*monitor(void *table_philos)
{
	t_table	*table;

	table = (t_table *)table_philos;
	while (keep_running(table))
	{
		if (everyone_ate(table))
			break ;
		if (philo_is_dead(table))
			break ;
		usleep(200);
	}
	return (NULL);
}

bool	everyone_ate(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
	{
		if (table->philosophers[i].eat_count < table->total_meals)
			return (false);
		i++;
	}
	pthread_mutex_lock(&table->death_mutex);
	table->all_ate = true;
	pthread_mutex_unlock(&table->death_mutex);
	return (true);
}

bool	keep_running(t_table *table)
{
	pthread_mutex_lock(&table->death_mutex);
	if (table->is_dead || table->all_ate)
	{
		pthread_mutex_unlock(&table->death_mutex);
		return (false);
	}
	pthread_mutex_unlock(&table->death_mutex);
	return (true);
}

void	*routine(void *philosopher)
{
	t_table			*table;
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	table = philo->table;
	while (keep_running(table))
	{
		if (table->num_philosophers == 1)
			return (handle_case_one(philo));
		if (philo->eat_count >= table->total_meals)
			break ;
		if (philo->eat_count < table->total_meals && get_forks(philo))
		{
			eating(philo);
			unlock_forks(philo);
			sleeping(philo);
			thinking(philo);
		}
	}
	if (philo->has_left_fork)
		pthread_mutex_unlock(philo->left_fork_mutex);
	if (philo->has_right_fork)
		pthread_mutex_unlock(philo->right_fork_mutex);
	return (NULL);
}

bool	philo_is_dead(t_table *table)
{
	int		i;
	long	time_now;
	long	last;

	i = 0;
	while (i < table->num_philosophers)
	{
		time_now = now_ms();
		pthread_mutex_lock(&table->death_mutex);
		last = table->philosophers[i].last_meal_time;
		pthread_mutex_unlock(&table->death_mutex);
		if (time_now - last >= table->time_to_die)
		{
			pthread_mutex_lock(&table->print_mutex);
			printf("%ld %d died\n", time_now - table->start_time,
				table->philosophers[i].id);
			pthread_mutex_unlock(&table->print_mutex);
			pthread_mutex_lock(&table->death_mutex);
			table->is_dead = true;
			pthread_mutex_unlock(&table->death_mutex);
			return (true);
		}
		i++;
	}
	return (false);
}
