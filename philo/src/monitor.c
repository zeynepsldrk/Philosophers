/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:32:26 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/07 21:45:02 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_anyone_dead(t_all *all)
{
	int i;
    long current_time;

    i = 0;
    while (i < all->number_of_philo)
    {
        current_time = time_in_ms();
        if ((current_time - all->philo[i].last_meal_time) > all->time_to_die) // a gloabal (struct) variable for checking is a philo died
        {
			pthread_mutex_lock(&all->print_mutex); // nobody unlocks the mutex (print)
            printf("%ld %d died\n", current_time - all->start_time, all->philo[i].philo_id);
            all->someone_died = 1;
			pthread_mutex_unlock(&all->print_mutex);
            return (1);
        }
        i++;
    }
    if (all_eat_enough(all))
        {
            all->someone_died = 1;
            return (1);
        }
    return (0);
}

int all_eat_enough(t_all *all)
{
    int i;

    if (all->times_each_philo_must_eat == -1)
        return (0);
    i = 0;
    while (i < all->number_of_philo)
    {
        if (all->philo[i].meal_count < all->times_each_philo_must_eat)
            return (0);
        i++;
    }
    return (1);
}

int status_check(t_philo *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    if (philo->all->someone_died)
    {
        if (first_fork)
            pthread_mutex_unlock(first_fork);
        if (second_fork)
            pthread_mutex_unlock(second_fork);
        return 1;
    }
    return 0;
}
void let_time_pass(t_philo *philo, long action_time, char *str)
{
	if(str_cmp(str, "sleeping") == 0)
	{
		while ((time_in_ms() - action_time) < philo->all->time_to_sleep)
		{
			if (philo->all->someone_died)
				break;
			usleep(500);
		}
	}
	if(str_cmp(str, "eating") == 0)
	{
		while ((time_in_ms() - action_time) < philo->all->time_to_eat)
		{
			if (philo->all->someone_died)
				break;
			usleep(500);
		}
	}
}
