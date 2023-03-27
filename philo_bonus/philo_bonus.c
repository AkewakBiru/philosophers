/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:23:47 by abiru             #+#    #+#             */
/*   Updated: 2023/03/27 15:10:01 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// only wait for one process, because if sth exited, that means a philo has died
void	routine(t_info *philos, sem_t *sem_lock, sem_t *sem_print)
{
	philos->status = ALIVE;
	philos->start_time = get_time();
	philos->last_ate = get_time();
	while (check_death(philos) != DEAD)
	{
		// take the first fork
		sem_wait(sem_lock);
		// print msg
		sem_wait(sem_print);
		printf("%s[%lu] %d has taken a fork\n%s", COL_FORK, get_time() - philos->start_time, philos->id, COL_DFL);
		sem_post(sem_print);
		// take the second fork
		sem_wait(sem_lock);
		// print msg
		sem_wait(sem_print);
		printf("%s[%lu] %d has taken a fork\n%s", COL_FORK, get_time() - philos->start_time, philos->id, COL_DFL);
		sem_post(sem_print);
		// eat
		sem_wait(sem_print);
		printf("%s[%lu] %d is eating\n%s", COL_EAT, get_time() - philos->start_time, philos->id, COL_DFL);
		sem_post(sem_print);
		if (!wait_action(get_time(), philos->time_to_eat, philos))
			exit(philos->id);
		// release forks
		sem_post(sem_lock);
		sem_post(sem_lock);
		// sleep
		sem_wait(sem_print);
		printf("%s[%lu] %d is sleeping\n%s", COL_SLEEP, get_time() - philos->start_time, philos->id, COL_DFL);
		sem_post(sem_print);
		philos->last_ate = get_time();
		if (!wait_action(get_time(), philos->time_to_sleep, philos))
			exit(philos->id);
		// think
		sem_wait(sem_print);
		printf("%s[%lu] %d is thinking\n%s", COL_THINK, get_time() - philos->start_time, philos->id, COL_DFL);
		sem_post(sem_print);
	}
	// printf("inside the routine func\n");
	exit(philos->id);
}

int	main(int ac, char **av)
{
	t_info	philos;
	sem_t	*semaphores;
	sem_t	*sem_action;
	int		i;
	pid_t	proc;
	int		e_status;

	validate_input(&philos, ac, av);
	semaphores = sem_open("f_sem", O_CREAT | O_EXCL, philos.num_philo);
	sem_action = sem_open("p_sem", O_CREAT | O_EXCL, 1);
	i = -1;
	philos.id = 0;
	e_status = 0;
	philos.start_time = get_time();
	while (++i < philos.num_philo)
	{
		proc = fork();
		if (proc == -1)
			perror("Fork");
		if (proc == 0)
		{
			philos.id = i + 1;
			routine(&philos, semaphores, sem_action);
			exit(EXIT_SUCCESS);
		}
	}
	sem_close(semaphores);
	waitpid(-1, &e_status, 0);
	if (WIFEXITED(e_status))
		e_status = WEXITSTATUS(e_status);
	else if (WIFSIGNALED(e_status))
		e_status = WTERMSIG(e_status);
	printf("[%lu] %d died\n", get_time() - philos.start_time, e_status);
	return (e_status);
}
