/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:32:26 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/14 20:23:11 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_anyone_dead(t_all *all, int i)
{
    long current_time;

    while (i < all->number_of_philo)
    {
		pthread_mutex_lock(&all->philo[i].meal_mutex);
        current_time = time_in_ms();
        if ((current_time - all->philo[i].last_meal_time) > all->time_to_die)
        {
			pthread_mutex_lock(&all->print_mutex);
            printf("%ld %d died\n", current_time - all->start_time, all->philo[i].philo_id);
			pthread_mutex_lock(&all->someone_died_mutex);
            all->someone_died = 1;
			pthread_mutex_unlock(&all->someone_died_mutex);
			pthread_mutex_unlock(&all->print_mutex);
            pthread_mutex_lock(&all->philo[i].meal_mutex);
            return (1);
        }
		pthread_mutex_unlock(&all->philo[i].meal_mutex);
        i++;
    }
	return (status_check(all->philo, NULL, NULL));
}

int all_eat_enough(t_all *all)
{
    int i;

    if (all->times_each_philo_must_eat == -1)
        return (0);
    i = 0;
    while (i < all->number_of_philo)
    {
		pthread_mutex_lock(&all->philo->meal_mutex);
        if (all->philo[i].meal_count < all->times_each_philo_must_eat)
		{
			pthread_mutex_unlock(&all->philo->meal_mutex);
			return (0);
		}
		pthread_mutex_unlock(&all->philo->meal_mutex);
        i++;
    }
    return (1);
}

int status_check(t_philo *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    pthread_mutex_lock(&philo->all->someone_died_mutex);
	if (philo->all->someone_died)
	{
		if (first_fork)
            pthread_mutex_unlock(first_fork);
        if (second_fork)
            pthread_mutex_unlock(second_fork);
        pthread_mutex_unlock(&philo->all->someone_died_mutex);
		return 1;
    }
	pthread_mutex_unlock(&philo->all->someone_died_mutex);
	if (all_eat_enough(philo->all))
    {
		pthread_mutex_lock(&philo->all->someone_died_mutex);
        philo->all->someone_died = 1;
		pthread_mutex_unlock(&philo->all->someone_died_mutex);
		if (first_fork)
            pthread_mutex_unlock(first_fork);
        if (second_fork)
            pthread_mutex_unlock(second_fork);
    	return (1);
    }
    return 0;
}

void let_time_pass(t_philo *philo, long action_time)
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
		usleep(500);
	}
}

int	ft_lonely_eating(t_philo *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	which_fork_first(philo, &first_fork, &second_fork);
	pthread_mutex_lock(first_fork);
	if (status_check(philo, first_fork, NULL))
		return 1;
	pthread_mutex_lock(&philo->all->print_mutex);
	printf("%ld %d has taken a fork\n", time_in_ms() - philo->all->start_time, philo->philo_id);
	pthread_mutex_unlock(&philo->all->print_mutex);
	pthread_mutex_lock(second_fork);
	if (status_check(philo, first_fork, second_fork))
		return 1;
	pthread_mutex_lock(&philo->all->print_mutex);
	printf("%ld %d has taken a fork\n", time_in_ms() - philo->all->start_time, philo->philo_id);
	pthread_mutex_unlock(&philo->all->print_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = time_in_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	print_action("eating", philo, philo->last_meal_time);
    pthread_mutex_lock(&philo->meal_mutex);
	philo->meal_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
	return 0;
}
