/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:49:07 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/13 20:46:38 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(char *str, t_philo *philo, long action_time)
{
	pthread_mutex_lock(&philo->all->print_mutex);
    if(philo->all->someone_died)
	{
		pthread_mutex_unlock(&philo->all->print_mutex);
		return ;
	}
    if(str_cmp(str, "eating") == 0)
		printf("%ld %d is eating\n", action_time - philo->all->start_time, philo->philo_id);
	if(str_cmp(str, "sleeping") == 0)
		printf("%ld %d is sleeping\n", action_time - philo->all->start_time, philo->philo_id);
	if(str_cmp(str, "thinking") == 0)
		printf("%ld %d is thinking\n", time_in_ms() - philo->all->start_time, philo->philo_id);
	pthread_mutex_unlock(&philo->all->print_mutex);
	let_time_pass(philo, action_time);
}

void make_action(t_philo *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	while (!philo->all->someone_died)
	{
		if(ft_lonely_eating(philo, first_fork, second_fork))
			break;
        philo->sleep_start = time_in_ms();
        print_action("sleeping", philo, philo->sleep_start);
        pthread_mutex_lock(&philo->all->someone_died_mutex);
		if (philo->all->someone_died)
		{
			pthread_mutex_unlock(&philo->all->someone_died_mutex);
            break;
		}
        print_action("thinking", philo, 0);
	}
}

void	*start_routine(void *arg)
{
	t_philo *philo;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    first_fork = NULL;
    second_fork = NULL;
	philo = (t_philo*)arg;
    if (philo->all->number_of_philo == 1)
    {
        only_one_philo(philo);
        return NULL;
    }
    if (philo->all->number_of_philo % 2 == 1 && !(philo->all->number_of_philo == 1))
    {
        if(philo->all->number_of_philo == philo->philo_id)
            usleep(philo->all->time_to_eat * 500); // already waiting for others to drop forks
    }
    make_action(philo, first_fork, second_fork);
	return (NULL);
}

void	start_philosophers(t_all *all, long number_of_philo)
{
	int i;

	i = 0;
    all->philo = malloc(sizeof(t_philo)*number_of_philo);
    if (malloc_error(all->philo, NULL))
		return ;
    all->forks = malloc(sizeof(pthread_mutex_t)*number_of_philo);
    if (malloc_error(all->forks, all->philo))
	    return ;
	all->start_time = time_in_ms();
	pthread_mutex_init(&all->print_mutex, NULL);
	pthread_mutex_init(&all->someone_died_mutex, NULL);
	pthread_mutex_init(&all->philo->meal_mutex, NULL);
    init_threads_mutexs(all, number_of_philo, i);
	free_and_destroy(all);
}

int	main(int argc, char **argv)
{
	int		i;
	t_all	all;
	
	if (argc != 5 && argc != 6)
	{
		write(2, "Wrong argument number!\n", 24);
		return (1);
	}
    i = 1;
    while (argv[i])
    {
        if (!ft_isdigit(argv[i]))
            write(2, "Invalid argument!\n", 19);
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
