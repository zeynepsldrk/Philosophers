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
