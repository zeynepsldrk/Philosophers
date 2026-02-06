/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bebek.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:05:41 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/04 15:49:30 by zedurak          ###   ########.fr       */
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
        if ((current_time - all->philo[i].last_meal_time) > all->time_to_die)
        {
            printf("%ld %d died\n", current_time - all->start_time, all->philo[i].philo_id);
            all->someone_died = 1;
            return (1);
        }
        i++;
        if (all_eat_enough(all))
        {
            all->someone_died = 1;
            return (1);
        }
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