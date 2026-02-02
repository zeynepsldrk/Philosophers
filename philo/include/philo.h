/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 16:47:11 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/03 15:47:57 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <limits.h>

typedef struct s_philo t_philo;

typedef struct s_all_things
{
    int number_of_philo;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    int times_each_philo_must_eat;
    t_philo *philo;
    pthread_mutex_t *forks;
    long start_time;
	int someone_died;
} t_all;

typedef struct s_philo
{
    pthread_t philos;
    int philo_id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
<<<<<<< HEAD
	int l_fork_id;
	int r_fork_id;
=======
    int left_fork_id;
    int right_fork_id;
>>>>>>> 948bb67 (add forks id for share the forks)
    long meal_count;
    t_all *all;
} t_philo;


int		ft_isdigit(char *str);
long	ft_modified_atol(char *str);
int		str_cmp(char *s1, char *s2);
void	free_and_destroy(t_all *all);
void	start_philosophers(t_all *all, long number_of_philo);
void	init_threads(int i, t_all *all, int number_of_philo);
void	*start_routine(void *arg);
void	make_action(char *str, t_philo *philo);
long	time_in_ms(void);

#endif
