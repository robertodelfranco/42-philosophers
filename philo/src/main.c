/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:53:34 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/04/29 17:46:28 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	parse_args(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (printf("Error: Invalid number of arguments.\n"));
	i = 1;
	while (i < argc)
	{
		if (ft_atol(argv[i]) <= 0)
			return (printf("Error: All arguments must be positive numbers.\n"),
				false);
		i++;
	}
	return (true);
}

int	main(int argc, char **argv)
{
	t_table			*table;

	if (!parse_args(argc, argv))
		return (1);
	table = ft_calloc(1, sizeof(t_table));
	if (!table)
		return (printf("Error: Memory allocation failed.\n"), 1);
	init_table(table, argv);
	if (table->num_philosophers < MIN_PHILOSOPHERS
		|| table->num_philosophers > MAX_PHILOSOPHERS)
		return (printf("Error: Number of philos must be between %d and %d\n",
				MIN_PHILOSOPHERS, MAX_PHILOSOPHERS), 1);
	table->philosophers = (t_philosopher *)ft_calloc(table->num_philosophers,
			sizeof(t_philosopher));
	table->forks = (pthread_mutex_t *)ft_calloc(table->num_philosophers,
			sizeof(pthread_mutex_t));
	init_all(table);
	clean_mutex(table);
	free(table->philosophers);
	free(table->forks);
	free(table);
}

void	unlock_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(philo->left_fork_mutex);
	philo->has_left_fork = false;
	pthread_mutex_unlock(philo->right_fork_mutex);
	philo->has_right_fork = false;
}

void	*handle_case_one(t_philosopher *philo)
{
	long	start_time;
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(&table->forks[philo->left_fork]);
	pthread_mutex_lock(&table->print_mutex);
	printf("%ld %d has taken a fork\n",
		now_ms() - table->start_time, philo->id);
	pthread_mutex_unlock(&table->print_mutex);
	start_time = now_ms();
	while (keep_running(table) && now_ms() - start_time < table->time_to_die)
		usleep(100);
	pthread_mutex_unlock(&table->forks[philo->left_fork]);
	return (NULL);
}
