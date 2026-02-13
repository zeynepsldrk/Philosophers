/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:05:41 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/13 16:28:03 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    only_one_philo(t_philo *philo)
{
    pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&philo->all->print_mutex);
    printf("%ld %d has taken a fork\n", time_in_ms() - philo->all->start_time, philo->philo_id);
	pthread_mutex_unlock(&philo->all->print_mutex);
    while (!philo->all->someone_died)
        usleep(1000);
    pthread_mutex_unlock(philo->left_fork);
}

void which_fork_first(t_philo *philo, pthread_mutex_t **first_fork, pthread_mutex_t **second_fork)
{
    if (philo->l_fork_id < philo->r_fork_id)
    {
        *first_fork = philo->left_fork;
        *second_fork = philo->right_fork;
    }
    else
    {
        *first_fork = philo->right_fork;
        *second_fork = philo->left_fork;
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
