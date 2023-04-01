/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:49:57 by abiru             #+#    #+#             */
/*   Updated: 2023/04/01 16:00:56 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	terminate_threads(t_info *philos, int i)
{
	write(2, "Thread creation failed\n", 23);
	while (i-- >= 0)
		pthread_detach(philos->philo[i].id);
	finish_exec(philos);
	return (EXIT_FAILURE);
}

void	single_philo(t_info *philos)
{
	if (philos->num_philo != 1)
		return ;
	philos->philo[0].num = 1;
	philos->philo[0].p_info = philos;
	philos->philo[0].left_fork = &(philos->forks[0]);
	philos->philo[0].right_fork = &(philos->forks[0]);
	philos->philo[0].l_val = &(philos->f[0]);
	philos->philo[0].r_val = &(philos->f[0]);
	if (pthread_create(&philos->philo[0].id, 0,
			handle_one_philo, philos->philo + 0) != 0)
		return (perror("Thread"), finish_exec(philos));
	pthread_join(philos->philo[0].id, 0);
	finish_exec(philos);
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

/*
	trying to unlock a mutex that is already unlocked
*/
void	unlock_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	*philo->r_val = 0;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(philo->left_fork);
	*philo->l_val = 0;
	pthread_mutex_unlock(philo->left_fork);
}

void	wait_left_fork(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(philo->left_fork);
		if (*philo->l_val == 0)
			break ;
		pthread_mutex_unlock(philo->left_fork);
		usleep(500);
	}
}
