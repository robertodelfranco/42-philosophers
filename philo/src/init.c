/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:30:51 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/04/29 17:23:39 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	init_all(t_table *table)
{
	int	i;

	table->start_time = now_ms();
	i = 0;
	while (i < table->num_philosophers)
	{
		table->philosophers[i].last_meal_time = table->start_time;
		i++;
	}
	init_philosophers(table);
	init_mutex(table);
	pthread_create(&table->monitor_thread, NULL, monitor, table);
	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_create(&table->philosophers[i].thread, NULL, routine,
			&table->philosophers[i]);
		i++;
	}
	pthread_join(table->monitor_thread, NULL);
	i = 0;
	while (i < table->num_philosophers)
		pthread_join(table->philosophers[i++].thread, NULL);
}

void	init_table(t_table *table, char **argv)
{
	table->num_philosophers = ft_atoi(argv[1]);
	table->time_to_die = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	if (argv[5])
		table->total_meals = ft_atoi(argv[5]);
	else
		table->total_meals = LONG_MAX;
	table->is_dead = false;
	table->all_ate = false;
	if ((table->time_to_die - (table->time_to_sleep + table->time_to_eat)) - 1
		<= 0)
		table->time_to_think = 0;
	else
		table->time_to_think = (table->time_to_die - (table->time_to_sleep
					+ table->time_to_eat)) - 1;
}

void	init_mutex(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->death_mutex, NULL);
}

void	init_philosophers(void *table_philos)
{
	int				i;
	t_table			*table;

	table = (t_table *)table_philos;
	i = 0;
	while (i < table->num_philosophers)
	{
		table->philosophers[i].id = i + 1;
		table->philosophers[i].left_fork = i;
		table->philosophers[i].right_fork = (i + 1) % table->num_philosophers;
		table->philosophers[i].eat_count = 0;
		table->philosophers[i].table = table;
		table->philosophers[i].left_fork_mutex
			= &table->forks[table->philosophers[i].left_fork];
		table->philosophers[i].right_fork_mutex
			= &table->forks[table->philosophers[i].right_fork];
		table->philosophers[i].has_left_fork = false;
		table->philosophers[i].has_right_fork = false;
		i++;
	}
}
