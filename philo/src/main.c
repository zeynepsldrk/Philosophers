/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:49:07 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/03 16:48:39 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	make_action(char *str, t_philo *philo)
{
	if(str_cmp(str, "eating") == 0)
	{
		if (philo->l_fork_id < philo->r_fork_id)
			printf("%d and %d forks taken\n", philo->l_fork_id, philo->r_fork_id);
		else
			printf("%d and %d forks taken\n", philo->r_fork_id, philo->l_fork_id);
		printf("I'm eating..\n");
	}
	if(str_cmp(str, "sleeping") == 0)
	{
		if (philo->l_fork_id < philo->r_fork_id)
			printf("%d and %d forks put down\n", philo->l_fork_id, philo->r_fork_id);
		else
			printf("%d and %d forks put down\n", philo->r_fork_id, philo->l_fork_id);
		printf("I'm sleeping..\n");
	}
	if(str_cmp(str, "thinking") == 0)
		write(1, "I'm thinking..\n", 16);
}

void	*start_routine(void *arg)
{
	t_philo *philo;
	long eat_start;
	long sleep_start;

	philo = (t_philo*)arg;
	while (!philo->all->someone_died)
	{
		if (philo->l_fork_id < philo->r_fork_id)
		{
			pthread_mutex_lock(philo->left_fork);
			pthread_mutex_lock(philo->right_fork);
			eat_start = time_in_ms();
			make_action("eating", philo);
			while (philo->all->someone_died == 0 && (time_in_ms() - eat_start) < philo->all->time_to_eat)
				usleep(100);
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			sleep_start = time_in_ms();
			make_action("sleeping", philo);
			while (philo->all->someone_died == 0 && (time_in_ms() - sleep_start) < philo->all->time_to_sleep)
				usleep(100);
			make_action("thinking", philo);
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			pthread_mutex_lock(philo->left_fork);
			eat_start = time_in_ms();
			make_action("eating", philo);
			while (philo->all->someone_died == 0 && (time_in_ms() - eat_start) < philo->all->time_to_eat)
				usleep(100);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			sleep_start = time_in_ms();
			make_action("sleeping", philo);
			while (philo->all->someone_died == 0 && (time_in_ms() - sleep_start) < philo->all->time_to_sleep)
				usleep(100);
			make_action("thinking", philo);
		}
	}
	return (NULL);
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
}

void	start_philosophers(t_all *all, long number_of_philo)
{
    int i;
    int count;

    count = number_of_philo;
    i = 0;
    all->philo = malloc(sizeof(t_philo)*number_of_philo);
    all->forks = malloc(sizeof(pthread_mutex_t)*number_of_philo);
	all->start_time = time_in_ms();
    while (count > 0)
    {
		pthread_mutex_init(&all->forks[i], NULL);
		init_threads(i, all, number_of_philo);
        pthread_create(&all->philo[i].philos, NULL, start_routine, &all->philo[i]);
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
