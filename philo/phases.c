/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phases.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 23:00:53 by abiru             #+#    #+#             */
/*   Updated: 2023/04/01 16:08:13 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eat(int num, t_info *global, t_philo *philo)
{
	unsigned long	start;

	if (check_status(global))
		return (unlock_forks(philo), 0);
	pthread_mutex_lock(&global->print_mutex);
	printf("\033[0;33m[%lu] %d is eating\n\033[0;30m",
		get_time() - global->start_time, num);
	pthread_mutex_unlock(&global->print_mutex);
	start = get_time();
	pthread_mutex_lock(&global->r_mutex);
		philo->last_ate = start;
	pthread_mutex_unlock(&global->r_mutex);
	if (!wait_action(get_time(), global->time_to_eat, global))
		return (unlock_forks(philo), 0);
	pthread_mutex_lock(&global->r_mutex);
	philo->num_eat++;
	pthread_mutex_unlock(&global->r_mutex);
	return (1);
}

int	ft_sleep(int id, t_info *global)
{
	if (check_status(global))
		return (0);
	pthread_mutex_lock(&global->print_mutex);
	printf("\033[0;36m[%lu] %d is sleeping\n\033[0;30m",
		get_time() - global->start_time, id);
	pthread_mutex_unlock(&global->print_mutex);
	return (wait_action(get_time(), global->time_to_sleep, global));
}

int	think(t_info *global, t_philo *philo)
{
	if (check_status(global))
		return (0);
	pthread_mutex_lock(&global->print_mutex);
	printf("\033[0;35m[%lu] %d is thinking\n\033[0;30m",
		get_time() - global->start_time, philo->num);
	pthread_mutex_unlock(&global->print_mutex);
	return (1);
}

int	lock_left_fork(t_info *global, t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (*philo->l_val)
	{
		pthread_mutex_unlock(philo->left_fork);
		if (check_status(global))
		{
			pthread_mutex_lock(philo->left_fork);
			*philo->l_val = 0;
			pthread_mutex_unlock(philo->left_fork);
			return (0);
		}
		wait_left_fork(philo);
	}
	*philo->l_val = 1;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&global->print_mutex);
	printf("\033[0;32m[%lu] %d has taken left fork\n\033[0;30m",
		get_time() - global->start_time, philo->num);
	pthread_mutex_unlock(&global->print_mutex);
	return (1);
}

int	lock_right_fork(t_info *global, t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	if (*philo->r_val == 1)
	{
		pthread_mutex_unlock(philo->right_fork);
		if (check_status(global))
			return (unlock_forks(philo), 0);
		while (1)
		{
			pthread_mutex_lock(philo->right_fork);
			if (*philo->r_val == 0)
				break ;
			pthread_mutex_unlock(philo->right_fork);
			usleep(500);
		}
	}
	*philo->r_val = 1;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&global->print_mutex);
	printf("\033[0;32m[%lu] %d has taken right fork\n\033[0;30m",
		get_time() - global->start_time, philo->num);
	pthread_mutex_unlock(&global->print_mutex);
	return (1);
}
