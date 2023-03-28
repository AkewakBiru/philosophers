/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:23:47 by abiru             #+#    #+#             */
/*   Updated: 2023/03/28 22:33:08 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <signal.h>

void	*check_status(void *data)
{
	t_info	*philos;

	philos = (t_info *)data;
	while (!DEAD)
	{
		sem_wait(philos->sem_p);
		if (get_time() - philos->last_ate >= (unsigned long)philos->time_to_die)
		{
			philos->status = DEAD;
			printf("[%lu] %d died\n", get_time() - philos->start_time,
				philos->id);
			sem_post(philos->sem_d);
			return (0);
		}
		sem_post(philos->sem_p);
		usleep(100);
	}
	return (0);
}

// a thread that runs in an infinite loop is used to check if a philo dies
// and release a blocking semaphore so that the parent process kills every
// children procs
void	routine(t_info *philos)
{
	pthread_t	t_id;

	philos->last_ate = get_time();
	pthread_create(&t_id, 0, check_status, philos);
	pthread_detach(t_id);
	while (1)
	{
		if (philos->num_eat != 2147483649 && philos->eat_count
			>= philos->num_eat)
		{
			sem_post(philos->sem_d);
			break ;
		}
		get_forks(philos);
		eat(philos);
		sem_post(philos->sem_f);
		sem_post(philos->sem_f);
		ft_sleep(philos);
		sem_wait(philos->sem_p);
		printf("%s[%lu] %d is thinking\n%s", COL_THINK, get_time()
			- philos->start_time, philos->id, COL_DFL);
		sem_post(philos->sem_p);
	}
	exit(EXIT_SUCCESS);
}

void	handle_one_philo(t_info *philos)
{
	pid_t	p_id;

	p_id = fork();
	if (p_id == -1)
		perror("Fork");
	else if (p_id == 0)
	{
		sem_wait(philos->sem_f);
		printf("%s[%lu] %d has taken a fork\n%s", COL_FORK, get_time()
			- philos->start_time, 1, COL_DFL);
		usleep(philos->time_to_die * 1000);
		sem_post(philos->sem_f);
		printf("[%lu] %d died\n", get_time() - philos->start_time, 1);
		exit(0);
	}
	waitpid(p_id, 0, 0);
	kill(p_id, SIGKILL);
	close_semaphores(philos);
	exit(0);
}

int	main(int ac, char **av)
{
	t_info	philos;
	int		i;
	pid_t	proc[200];

	validate_input(&philos, ac, av);
	open_semaphores(&philos);
	philos.start_time = get_time();
	if (philos.num_philo == 1)
		handle_one_philo(&philos);
	init_procs(&philos, proc);
	sem_wait(philos.sem_d);
	i = -1;
	while (++i < philos.num_philo)
		kill(proc[i], SIGKILL);
	close_semaphores(&philos);
	return (EXIT_SUCCESS);
}
