/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 13:46:09 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/21 11:28:20 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

long	ft_modified_atol(char *str)
{
	long	result;
	int		last_digit;

	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		last_digit = *str - '0';
		if (result > (2147483647 - last_digit) / 10)
			return (-1);
		result = result * 10 + last_digit;
		str++;
	}
	return (result);
}

int	str_cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

long	time_in_ms(void)
{
	struct timeval	tv;
	long			milliseconds;

	gettimeofday(&tv, NULL);
	milliseconds = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (milliseconds);
}

void	free_and_destroy(t_all *all)
{
	int	i;
	int	count;

	count = all->number_of_philo;
	i = 0;
	while (count)
	{
		pthread_mutex_destroy(&all->forks[i]);
		pthread_mutex_destroy(&all->philo[i].meal_mutex);
		i++;
		count--;
	}
	pthread_mutex_destroy(&all->print_mutex);
	pthread_mutex_destroy(&all->someone_died_mutex);
	free(all->philo);
	free(all->forks);
}
