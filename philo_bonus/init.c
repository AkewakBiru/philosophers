/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 22:17:01 by abiru             #+#    #+#             */
/*   Updated: 2023/03/29 16:00:45 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	unlink_semaphores(void)
{
	sem_unlink("/f_sem");
	sem_unlink("/p_sem");
	sem_unlink("/sem_d");
	sem_unlink("/sem_check");
}

void	close_semaphores(t_info *philos)
{
	sem_close(philos->sem_p);
	sem_close(philos->sem_d);
	sem_close(philos->sem_f);
	sem_close(philos->sem_chk);
	unlink_semaphores();
}

void	open_semaphores(t_info *philos)
{
	unlink_semaphores();
	philos->sem_f = sem_open("/f_sem", O_CREAT | O_EXCL, 0777,
			philos->num_philo);
	philos->sem_p = sem_open("/p_sem", O_CREAT | O_EXCL, 0777, 1);
	philos->sem_d = sem_open("/sem_d", O_CREAT | O_EXCL, 0777, 0);
	philos->sem_chk = sem_open("/sem_check", O_CREAT | O_EXCL, 0777, 1);
	if (philos->sem_f == SEM_FAILED || philos->sem_p == SEM_FAILED
		|| philos->sem_d == SEM_FAILED || philos->sem_chk == SEM_FAILED)
		error_msg("Semaphore creation failed");
}

void	kill_siblings(t_info *philos, int proc[], int i)
{
	while (i-- >= 0)
		kill(proc[i], SIGKILL);
	close_semaphores(philos);
	exit(EXIT_FAILURE);
}

void	init_procs(t_info *philos, int proc[])
{
	int	i;

	i = -1;
	while (++i < philos->num_philo)
	{
		proc[i] = fork();
		if (proc[i] == -1)
		{
			perror("Fork");
			kill_siblings(philos, proc, i);
		}
		if (proc[i] == 0)
		{
			philos->status = ALIVE;
			philos->id = i + 1;
			philos->eat_count = 0;
			routine(philos);
			exit(EXIT_SUCCESS);
		}
		usleep(200);
	}
}
