/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:53:01 by abiru             #+#    #+#             */
/*   Updated: 2023/03/29 14:26:47 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	wait_action(unsigned long start, unsigned long time, t_info *global)
{
	while (get_time() - start < time)
	{
		if (check_death(global) == DEAD)
			return (0);
		usleep(200);
	}
	return (1);
}

int	check_death(t_info *global)
{
	int	st;

	sem_wait(global->sem_chk);
	st = global->status;
	sem_post(global->sem_chk);
	return (st);
}

unsigned long	get_time(void)
{
	unsigned long	tm;
	struct timeval	time;

	gettimeofday(&time, 0);
	tm = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (tm);
}
