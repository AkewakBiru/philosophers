/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:53:01 by abiru             #+#    #+#             */
/*   Updated: 2023/03/27 14:17:54 by abiru            ###   ########.fr       */
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
	if (get_time() - global->last_ate >= (unsigned long)global->time_to_die)
		global->status = DEAD;
	return (global->status);
}

unsigned long	get_time(void)
{
	unsigned long	tm;
	struct timeval	time;

	gettimeofday(&time, 0);
	tm = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (tm);
}
