/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:05:41 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/15 15:03:45 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	only_one_philo(t_philo *philo)
{
	int	id;

	id = philo->philo_id;
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&philo->all->print_mutex);
	printf("%ld %d %s\n", time_in_ms() - philo->all->start_time, id, A);
	pthread_mutex_unlock(&philo->all->print_mutex);
	pthread_mutex_lock(&philo->all->someone_died_mutex);
	while (!philo->all->someone_died)
	{
		pthread_mutex_unlock(&philo->all->someone_died_mutex);
		usleep(1000);
		pthread_mutex_lock(&philo->all->someone_died_mutex);
	}
	pthread_mutex_unlock(&philo->all->someone_died_mutex);
	pthread_mutex_unlock(philo->left_fork);
}

void	which_fork(t_philo *p, pthread_mutex_t **f1, pthread_mutex_t **f2)
{
	if (p->l_fork_id < p->r_fork_id)
	{
		*f1 = p->left_fork;
		*f2 = p->right_fork;
	}
	else
	{
		*f1 = p->right_fork;
		*f2 = p->left_fork;
	}
}

void	init_threads(int i, t_all *all, int number_of_philo)
{
	all->philo[i].philo_id = i + 1;
	all->philo[i].left_fork = &all->forks[i];
	all->philo[i].right_fork = &all->forks[(i + 1) % number_of_philo];
	all->philo[i].l_fork_id = i;
	all->philo[i].r_fork_id = (i + 1) % number_of_philo;
	all->philo[i].meal_count = 0;
	all->philo[i].all = all;
	all->someone_died = 0;
}

int	malloc_error(void *philos, void *forks)
{
	if (!philos)
	{
		write(2, "Error: Malloc failed!\n", 21);
		if (forks)
			free(forks);
		return (1);
	}
	return (0);
}

void	init_threads_mutexs(t_all *all, long number_of_philo, int i)
{
	while (i < number_of_philo)
	{
		init_threads(i, all, number_of_philo);
		pthread_mutex_init(&all->forks[i], NULL);
		pthread_mutex_init(&all->philo[i].meal_mutex, NULL);
		all->philo[i].last_meal_time = all->start_time;
		i++;
	}
	i = 0;
	while (i < number_of_philo)
	{
		pthread_create(&all->philo[i].philos, NULL, start, &all->philo[i]);
		i++;
	}
	while (!is_anyone_dead(all, 0))
		usleep(1000);
	pthread_mutex_lock(&all->someone_died_mutex);
	all->someone_died = 1;
	pthread_mutex_unlock(&all->someone_died_mutex);
	i = 0;
	while (i < number_of_philo)
	{
		pthread_join(all->philo[i].philos, NULL);
		i++;
	}
}
