/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phases.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 22:32:48 by abiru             #+#    #+#             */
/*   Updated: 2023/03/29 14:30:56 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	get_forks(t_info *philos)
{
	sem_wait(philos->sem_f);
	sem_wait(philos->sem_p);
	printf("%s[%lu] %d has taken a fork\n%s", COL_FORK, get_time()
		- philos->start_time, philos->id, COL_DFL);
	sem_post(philos->sem_p);
	sem_wait(philos->sem_f);
	sem_wait(philos->sem_p);
	printf("%s[%lu] %d has taken a fork\n%s", COL_FORK, get_time()
		- philos->start_time, philos->id, COL_DFL);
	sem_post(philos->sem_p);
}

void	eat(t_info *philos)
{
	sem_wait(philos->sem_p);
	philos->last_ate = get_time();
	printf("%s[%lu] %d is eating\n%s", COL_EAT, get_time()
		- philos->start_time, philos->id, COL_DFL);
	sem_post(philos->sem_p);
	if (!wait_action(get_time(), philos->time_to_eat, philos))
		exit(philos->id);
	philos->eat_count++;
}

void	ft_sleep(t_info *philos)
{
	sem_wait(philos->sem_p);
	printf("%s[%lu] %d is sleeping\n%s", COL_SLEEP, get_time()
		- philos->start_time, philos->id, COL_DFL);
	sem_post(philos->sem_p);
	if (!wait_action(get_time(), philos->time_to_sleep, philos))
		exit(philos->id);
}
