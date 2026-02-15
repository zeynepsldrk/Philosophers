/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:49:07 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/15 15:03:50 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(char *str, t_philo *philo, long action_time)
{
	int	id;

	id = philo->philo_id;
	pthread_mutex_lock(&philo->all->print_mutex);
	if (philo->all->someone_died)
	{
		pthread_mutex_unlock(&philo->all->print_mutex);
		return ;
	}
	if (str_cmp(str, "is thinking") == 0)
		printf("%ld %d %s\n", time_in_ms() - philo->all->start_time, id, B);
	else
		printf("%ld %d %s\n", action_time - philo->all->start_time, id, str);
	pthread_mutex_unlock(&philo->all->print_mutex);
	if (str_cmp(str, "is eating") == 0)
		let_time_pass(philo, philo->all->time_to_eat);
	if (str_cmp(str, "is sleeping") == 0)
		let_time_pass(philo, philo->all->time_to_sleep);
}

void	make_action(t_philo *philo, pthread_mutex_t *f1, pthread_mutex_t *f2)
{
	while (1)
	{
		pthread_mutex_lock(&philo->all->someone_died_mutex);
		if (philo->all->someone_died)
		{
			pthread_mutex_unlock(&philo->all->someone_died_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->all->someone_died_mutex);
		if (ft_eating(philo, f1, f2))
			break ;
		philo->sleep_start = time_in_ms();
		print_action("is sleeping", philo, philo->sleep_start);
		print_action("is thinking", philo, time_in_ms());
	}
}

void	*start(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	philo = (t_philo *)arg;
	which_fork(philo, &first_fork, &second_fork);
	if (philo->all->number_of_philo == 1)
	{
		only_one_philo(philo);
		return (NULL);
	}
	if (philo->all->number_of_philo % 2 == 1)
	{
		//if (philo->all->number_of_philo == philo->philo_id)
			usleep(philo->all->time_to_eat * 500);
	}
	make_action(philo, first_fork, second_fork);
	return (NULL);
}

void	start_philosophers(t_all *all, long number_of_philo)
{
	int	i;

	i = 0;
	all->philo = malloc(sizeof(t_philo) * number_of_philo);
	if (malloc_error(all->philo, NULL))
		return ;
	all->forks = malloc(sizeof(pthread_mutex_t) * number_of_philo);
	if (malloc_error(all->forks, all->philo))
		return ;
	all->start_time = time_in_ms();
	pthread_mutex_init(&all->print_mutex, NULL);
	pthread_mutex_init(&all->someone_died_mutex, NULL);
	init_threads_mutexs(all, number_of_philo, i);
	free_and_destroy(all);
}

int	main(int argc, char **argv)
{
	int		i;
	t_all	all;

	if (argc != 5 && argc != 6)
		return (write(2, "Wrong argument number!\n", 24));
	i = 1;
	while (argv[i])
	{
		if (!ft_isdigit(argv[i]))
			return (write(2, "Invalid argument!\n", 19));
		i++;
	}
	all.number_of_philo = ft_modified_atol(argv[1]);
	all.time_to_die = ft_modified_atol(argv[2]);
	all.time_to_eat = ft_modified_atol(argv[3]);
	all.time_to_sleep = ft_modified_atol(argv[4]);
	if (argc == 6)
		all.times_each_philo_must_eat = ft_modified_atol(argv[5]);
	else
		all.times_each_philo_must_eat = -1;
	start_philosophers(&all, all.number_of_philo);
	return (0);
}
