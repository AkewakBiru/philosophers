/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 22:04:13 by abiru             #+#    #+#             */
/*   Updated: 2023/04/01 16:04:27 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	finish_exec(t_info *philos)
{
	int	i;

	i = -1;
	while (++i < philos->num_philo)
		pthread_mutex_destroy(&philos->forks[i]);
	pthread_mutex_destroy(&philos->print_mutex);
	pthread_mutex_destroy(&philos->r_mutex);
	pthread_mutex_destroy(&philos->d_mutex);
	free(philos->forks);
	free(philos->f);
	free(philos->philo);
	exit(EXIT_FAILURE);
}

int	check_status(t_info *global)
{
	int	status;

	pthread_mutex_lock(&global->d_mutex);
	status = global->end_sim;
	pthread_mutex_unlock(&global->d_mutex);
	return (status);
}

int	wait_action(unsigned long start, unsigned long time, t_info *global)
{
	while (get_time() - start < time)
	{
		if (check_status(global))
			return (0);
		usleep(200);
	}
	return (1);
}

/*
 	returns time in sec and microsecond converted to millisec
 */
unsigned long	get_time(void)
{
	unsigned long	tm;
	struct timeval	time;

	gettimeofday(&time, 0);
	tm = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (tm);
}

void	*handle_one_philo(void *d)
{
	t_philo	*philo;
	t_info	*global;

	philo = (t_philo *)d;
	global = philo->p_info;
	pthread_mutex_lock(philo->left_fork);
	*philo->l_val = 1;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&global->print_mutex);
	printf("\033[0;32m[%lu] %d has taken a fork\n\033[0;30m",
		get_time() - global->start_time, philo->num);
	pthread_mutex_unlock(&global->print_mutex);
	usleep(global->time_to_die * 1000);
	pthread_mutex_lock(philo->left_fork);
	*philo->l_val = 0;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&global->print_mutex);
	printf("\033[0;37m[%d] %d died\n", global->time_to_die, philo->num);
	pthread_mutex_unlock(&global->print_mutex);
	return (0);
}
