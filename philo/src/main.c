/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:49:07 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/01 14:18:31 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	make_action(char *str)
{
	if(str_cmp(str, "eating") == 0)
		write(1, "I'm eating..\n", 14);
	if(str_cmp(str, "sleeping") == 0)
		write(1, "I'm sleeping..\n", 16);
	if(str_cmp(str, "thinking") == 0)
		write(1, "I'm thinking..\n", 16);
}

void	*start_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	make_action("eating");
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	init_threads(int i, t_all *all, int number_of_philo)
{
	all->philo[i].philo_id = i + 1;
	all->philo[i].left_fork = &all->forks[i];
	all->philo[i].right_fork = &all->forks[(i + 1) % number_of_philo];
	all->philo[i].meal_count = 0;
	all->philo[i].all_things = all;
}

void	start_philosophers(t_all *all, long number_of_philo)
{
    int i;
    int count;

    count = number_of_philo;
    i = 0;
    all->philo = malloc(sizeof(t_philo)*number_of_philo);
    all->forks = malloc(sizeof(pthread_mutex_t)*number_of_philo);
    while (count > 0)
    {
		init_threads(i, &all, number_of_philo);
        pthread_create(&all->philo[i].philos, NULL, start_routine, &all->philo[i]);
        pthread_mutex_init(&all->forks[i], NULL);
        i++;
        count--;
    }
    count = number_of_philo;
    i = 0;
    while (count)
    {
        pthread_join(all->philo[i].philos, NULL);
        i++;
        count--;
    }
	free_and_destroy(all);
}

int	main(int argc, char **argv)
{
    if (argc != 5 && argc != 6)
	{
		write(2, "Wrong argument number!", 23);
		return (1);
	}
    int i;
    t_all all;

    i = 1;
    while (argv[i])
    {
        if (!ft_isdigit(argv[i]))
            write(2, "Invalid argument!", 18);
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
