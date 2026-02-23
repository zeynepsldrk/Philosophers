/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:32:26 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/16 13:53:56 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_anyone_dead(t_all *all, int i)
{
	long	curr_time;
	int		id;

	while (i < all->number_of_philo)
	{
		pthread_mutex_lock(&all->philo[i].meal_mutex);
		curr_time = time_in_ms();
		if ((curr_time - all->philo[i].last_meal_time) > all->time_to_die)
		{
			pthread_mutex_lock(&all->print_mutex);
			id = all->philo[i].philo_id;
			printf("%ld %d died\n", curr_time - all->start_time, id);
			pthread_mutex_lock(&all->someone_died_mutex);
			all->someone_died = 1;
			pthread_mutex_unlock(&all->someone_died_mutex);
			pthread_mutex_unlock(&all->print_mutex);
			pthread_mutex_unlock(&all->philo[i].meal_mutex);
			return (1);
		}
		pthread_mutex_unlock(&all->philo[i].meal_mutex);
		i++;
	}
	return (status_check(all->philo, NULL, NULL));
}

int	all_eat_enough(t_all *all)
{
	int	i;

	if (all->times_each_philo_must_eat == -1)
		return (0);
	i = 0;
	while (i < all->number_of_philo)
	{
		pthread_mutex_lock(&all->philo[i].meal_mutex);
		if (all->philo[i].meal_count < all->times_each_philo_must_eat)
		{
			pthread_mutex_unlock(&all->philo[i].meal_mutex);
			return (0);
		}
		pthread_mutex_unlock(&all->philo[i].meal_mutex);
		i++;
	}
	return (1);
}

int	status_check(t_philo *philo, pthread_mutex_t *f1, pthread_mutex_t *f2)
{
	pthread_mutex_lock(&philo->all->someone_died_mutex);
	if (philo->all->someone_died)
	{
		if (f1)
			pthread_mutex_unlock(f1);
		if (f2)
			pthread_mutex_unlock(f2);
		pthread_mutex_unlock(&philo->all->someone_died_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->all->someone_died_mutex);
	if (all_eat_enough(philo->all))
	{
		pthread_mutex_lock(&philo->all->someone_died_mutex);
		philo->all->someone_died = 1;
		pthread_mutex_unlock(&philo->all->someone_died_mutex);
		if (f1)
			pthread_mutex_unlock(f1);
		if (f2)
			pthread_mutex_unlock(f2);
		return (1);
	}
	return (0);
}

void	let_time_pass(t_philo *philo, long action_time)
{
	long	start_time;

	start_time = time_in_ms();
	while ((time_in_ms() - start_time) < action_time)
	{
		pthread_mutex_lock(&philo->all->someone_died_mutex);
		if (philo->all->someone_died)
		{
			pthread_mutex_unlock(&philo->all->someone_died_mutex);
			return ;
		}
		pthread_mutex_unlock(&philo->all->someone_died_mutex);
		usleep(100);
	}
}

int	ft_eating(t_philo *p, pthread_mutex_t *f1, pthread_mutex_t *f2)
{
	pthread_mutex_lock(f1);
	if (status_check(p, f1, NULL))
		return (1);
	print_action("has taken a fork", p, time_in_ms());
	pthread_mutex_lock(f2);
	if (status_check(p, f1, f2))
		return (1);
	print_action("has taken a fork", p, time_in_ms());
	pthread_mutex_lock(&p->meal_mutex);
	p->last_meal_time = time_in_ms();
	pthread_mutex_unlock(&p->meal_mutex);
	print_action("is eating", p, p->last_meal_time);
	pthread_mutex_lock(&p->meal_mutex);
	p->meal_count++;
	pthread_mutex_unlock(&p->meal_mutex);
	pthread_mutex_unlock(f2);
	pthread_mutex_unlock(f1);
	return (0);
}
