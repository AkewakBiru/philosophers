/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 07:54:49 by abiru             #+#    #+#             */
/*   Updated: 2023/03/30 17:12:43 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_info *global, t_philo *philo)
{
	if (get_time() - philo->last_ate >= (unsigned long)global->time_to_die)
	{
		global->end_sim = 1;
		return (1);
	}
	return (0);
}

void	*routine(void *d)
{
	t_philo	*philo;
	t_info	*global;

	philo = (t_philo *)d;
	global = philo->p_info;
	if (philo->num % 2 == 0)
		if (!handle_odd_philos(global, philo))
			return (0);
	while (!check_status(global))
	{
		if (!lock_left_fork(global, philo) || !lock_right_fork(global, philo))
			break ;
		if (!eat(philo->num, global, philo))
			break ;
		unlock_forks(philo);
		if (!ft_sleep(philo->num, global))
			break ;
		if (!think(global, philo))
			break ;
	}
	return (0);
}

int	check_num_ate(t_info *global)
{
	int	i;

	i = -1;
	while (++i < global->num_philo)
	{
		pthread_mutex_lock(&global->r_mutex);
		if (global->philo[i].num_eat >= global->num_eat)
		{
			pthread_mutex_unlock(&global->r_mutex);
			continue ;
		}
		pthread_mutex_unlock(&global->r_mutex);
		return (0);
	}
	pthread_mutex_lock(&global->d_mutex);
	global->end_sim = 1;
	pthread_mutex_unlock(&global->d_mutex);
	return (1);
}

int	check_philos(t_info	*philos)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < philos->num_philo)
		{
			pthread_mutex_lock(&philos->r_mutex);
			if (get_time() - philos->philo[i].last_ate
				>= (unsigned long)philos->time_to_die)
				return (print_death(philos, philos->philo[i].num));
			pthread_mutex_unlock(&philos->r_mutex);
		}
		if ((philos->num_eat != -1 && check_num_ate(philos)) || philos->end_sim)
			break ;
	}
	return (0);
}

/*
	parent checks statuses of philos and stops simulation when a philo dies
	or all philos eat for num_of_times_each_philos_must_eat times
*/
int	main(int ac, char **av)
{
	t_info	philos;
	int		k;

	validate_input(&philos, ac, av);
	alloc_mem(&philos);
	init_ints(&philos);
	if (!init_mutexes(&philos))
		return (free(philos.f), free(philos.forks), free(philos.philo),
			error_msg("Mutex creation failed\n"), 1);
	if (!init_philos(&philos))
		return (free(philos.f), free(philos.forks), free(philos.philo),
			error_msg("Thread creation failed\n"), 1);
	k = 0;
	if (philos.num_philo > 1)
		k = check_philos(&philos);
	wait_threads(&philos);
	if (k)
		printf("\033[0;37m[%lu] %d died\n", get_time()
			- philos.start_time, k);
	finish_exec(&philos);
}
