/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:49:57 by abiru             #+#    #+#             */
/*   Updated: 2023/03/29 15:50:35 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	terminate_threads(t_info *philos, int i)
{
	while (i-- >= 0)
		pthread_detach(philos->philo[i].id);
	free(philos->forks);
	free(philos->philo);
	return (EXIT_FAILURE);
}
