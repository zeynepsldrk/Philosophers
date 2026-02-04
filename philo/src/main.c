/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:49:07 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/04 16:12:57 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	make_action(char *str, t_philo *philo, long action_time)
{
    if(philo->all->someone_died)
        return ;
    if(str_cmp(str, "eating") == 0)
	{
		printf("%ld %d is eating\n", action_time - philo->all->start_time, philo->philo_id);
        while ((time_in_ms() - action_time) < philo->all->time_to_eat)
            usleep(500);
	}
	if(str_cmp(str, "sleeping") == 0)
	{
		printf("%ld %d is sleeping\n", action_time - philo->all->start_time, philo->philo_id);
        while ((time_in_ms() - action_time) < philo->all->time_to_sleep)
            usleep(500);
	}
	if(str_cmp(str, "thinking") == 0)
		printf("%ld %d is thinking\n", time_in_ms() - philo->all->start_time, philo->philo_id);
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
            printf("%ld %d has taken a fork\n", time_in_ms() - philo->all->start_time, philo->philo_id);
			pthread_mutex_lock(philo->right_fork);
            printf("%ld %d has taken a fork\n", time_in_ms() - philo->all->start_time, philo->philo_id);
			eat_start = time_in_ms();
			make_action("eating", philo, eat_start);
            philo->last_meal_time = time_in_ms(); //yemek bittinyo
            philo->meal_count++;
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			sleep_start = time_in_ms();
			make_action("sleeping", philo, sleep_start);
			make_action("thinking", philo, 0);
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
            printf("%ld %d has taken a fork\n", time_in_ms() - philo->all->start_time, philo->philo_id);
			pthread_mutex_lock(philo->left_fork);
            printf("%ld %d has taken a fork\n", time_in_ms() - philo->all->start_time, philo->philo_id);
			eat_start = time_in_ms();
			make_action("eating", philo, eat_start);
            philo->last_meal_time = time_in_ms();
            philo->meal_count++;
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			sleep_start = time_in_ms();
			make_action("sleeping", philo, sleep_start);
			make_action("thinking", philo, 0);
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
    all->philo[i].left_fork_id = i;
    all->philo[i].right_fork_id = (i + 1) % number_of_philo;
	all->philo[i].meal_count = 0;
	all->philo[i].all = all;
}

/*
join etmeden önce bir thread'in ölüp ölmediğini kontrol etmemiz gerekiyor. 
Eğer bir thread öldüyse, diğer thread'lerin de ölmesini sağlamamız gerekiyor. 
Bunu yapmak için, her thread'in son yemeğinden itibaren geçen süreyi kontrol edebiliriz. 
Eğer bu süre, time_to_die süresini aşarsa, o thread'in öldüğünü varsayabiliriz ve 
all->someone_died değişkenini 1 yaparak diğer thread'lerin de ölmesini sağlayabiliriz.
*/

void	start_philosophers(t_all *all, long number_of_philo)
{
    int i;

    i = 0;
    all->philo = malloc(sizeof(t_philo)*number_of_philo);
    all->forks = malloc(sizeof(pthread_mutex_t)*number_of_philo);
	all->start_time = time_in_ms();
    while (i < number_of_philo)
    {
		pthread_mutex_init(&all->forks[i], NULL);
		init_threads(i, all, number_of_philo);
        pthread_create(&all->philo[i].philos, NULL, start_routine, &all->philo[i]);
        i++;
    }
	while (!is_anyone_dead(all))
	{
		free_and_destroy(all);
		break;
	}
    i = 0;
    while (i < number_of_philo)
    {
        pthread_join(all->philo[i].philos, NULL);
        i++;
    }
	free_and_destroy(all);
}

int	main(int argc, char **argv)
{
    if (argc != 5 && argc != 6)
	{
		write(2, "Wrong argument number!\n", 24);
		return (1);
	}
    int i;
    t_all all;

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
