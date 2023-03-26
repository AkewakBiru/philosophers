/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 22:27:16 by abiru             #+#    #+#             */
/*   Updated: 2023/03/26 16:28:17 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutexes(t_info *philos)
{
	int	i;
	int	flag;

	i = -1;
	flag = 1;
	while (++i < philos->num_philo)
		if (pthread_mutex_init(&philos->forks[i], 0) != 0)
			flag = 0;
	if (pthread_mutex_init(&philos->print_mutex, 0) != 0)
		flag = 0;
	if (pthread_mutex_init(&philos->r_mutex, 0) != 0)
		flag = 0;
	if (pthread_mutex_init(&philos->d_mutex, 0) != 0)
		flag = 0;
	if (!flag)
		return (free(philos->forks), free(philos->philo), 0);
	philos->start_time = get_time();
	pthread_mutex_lock(&philos->d_mutex);
	philos->end_sim = 0;
	pthread_mutex_unlock(&philos->d_mutex);
	return (1);
}

int	init_philos(t_info *philos)
{
	int	i;

	i = -1;
	while (++i < philos->num_philo)
	{
		philos->philo[i].num = i + 1;
		philos->philo[i].is_alive = 1;
		philos->philo[i].last_ate = philos->start_time;
		philos->philo[i].num_eat = 0;
		philos->philo[i].p_info = philos;
		philos->philo[i].left_fork = &(philos->forks[i]);
		philos->philo[i].right_fork = &(philos->forks[(i + 1)
				% philos->num_philo]);
		if (pthread_create(&philos->philo[i].id, 0, routine,
				philos->philo + i) != 0)
		{
			free(philos->forks);
			free(philos->philo);
			return (0);
		}
	}
	return (1);
}

int	handle_odd_philos(t_info *global, t_philo *philo)
{
	if (!think(global, philo))
		return (0);
	usleep(global->time_to_eat * 100);
	return (1);
}

int	print_death(t_info *philos, int i)
{
	(void)i;
	pthread_mutex_lock(&philos->d_mutex);
	philos->end_sim = 1;
	pthread_mutex_unlock(&philos->d_mutex);
	pthread_mutex_unlock(&philos->r_mutex);
	return (i);
}

void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}
