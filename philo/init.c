/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 22:27:16 by abiru             #+#    #+#             */
/*   Updated: 2023/03/30 17:12:29 by abiru            ###   ########.fr       */
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
	single_philo(philos);
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
		philos->philo[i].l_val = &(philos->f[i]);
		philos->philo[i].r_val = &(philos->f[(i + 1)
				% philos->num_philo]);
		if (pthread_create(&philos->philo[i].id, 0, routine,
				philos->philo + i) != 0)
			return (terminate_threads(philos, i));
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

void	init_ints(t_info *philos)
{
	int	i;

	i = -1;
	while (++i < philos->num_philo)
		philos->f[i] = 0;
}

void	alloc_mem(t_info *philos)
{
	philos->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* philos->num_philo);
	if (!philos->forks)
		error_msg("Malloc failed\n");
	philos->f = (int *)malloc(sizeof(int) * philos->num_philo);
	if (!philos->f)
		return (free(philos->forks), error_msg("Malloc failed\n"));
	philos->philo = (t_philo *)malloc(sizeof(t_philo) * philos->num_philo);
	if (!philos->philo)
		return (free(philos->f), free(philos->forks),
			error_msg("Malloc failed\n"));
}
