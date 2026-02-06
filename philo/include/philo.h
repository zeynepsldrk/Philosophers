/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 16:47:11 by zedurak           #+#    #+#             */
/*   Updated: 2026/02/04 16:09:58 by zedurak          ###   ########.fr       */
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
	int l_fork_id;
	int r_fork_id;
    long meal_count;
	long last_meal_time;
    long eat_start;
    long sleep_start;
    t_all *all;
} t_philo;


int		ft_isdigit(char *str);
long	ft_modified_atol(char *str);
int		str_cmp(char *s1, char *s2);
void	free_and_destroy(t_all *all);
int	malloc_error(void *philos, void *forks);
void	start_philosophers(t_all *all, long number_of_philo);
void	init_threads(int i, t_all *all, int number_of_philo);
void	*start_routine(void *arg);
void	print_action(char *str, t_philo *philo, long action_time);
long	time_in_ms(void);
int all_eat_enough(t_all *all);
int	is_anyone_dead(t_all *all);
void    only_one_philo(t_philo *philo);
void make_action(t_philo *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);
void which_fork_first(t_philo *philo, pthread_mutex_t **first_fork, pthread_mutex_t **second_fork);
int status_check(t_philo *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);

#endif
