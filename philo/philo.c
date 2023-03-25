/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 07:54:49 by abiru             #+#    #+#             */
/*   Updated: 2023/03/25 19:54:05 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	finish_exec(t_info *philos);

void	handle_one_philo(t_philo *philo, t_info *global)
{
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&global->print_mutex);
	printf("[%lu] %d has taken a fork\n", get_time() - global->start_time, philo->num);
	pthread_mutex_unlock(&global->print_mutex);
	usleep(global->time_to_die * 1000);
	pthread_mutex_lock(&global->print_mutex);
	printf("[%d] %d died\n", global->time_to_die, philo->num);
	pthread_mutex_unlock(&global->print_mutex);
	finish_exec(global);
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
		// philos die when trying more than 70 sth, so changed the sleep value from 10 to 50;
		usleep(50);
	}
	return (1);
}

void	finish_exec(t_info *philos)
{
	int	i;

	i = -1;
	while (++i < philos->num_philo)
		pthread_mutex_destroy(&philos->forks[i]);
	pthread_mutex_unlock(&philos->print_mutex);
	pthread_mutex_destroy(&philos->print_mutex);
	pthread_mutex_destroy(&philos->r_mutex);
	pthread_mutex_destroy(&philos->d_mutex);
	i = -1;
	while (++i < philos->num_philo)
		pthread_detach(philos->philo[i].id);
	free(philos->forks);
	free(philos->philo);
	exit(1);
}

/*
	./philo 2 400 300 200

	Assume philo 1 picks up fork first
	cur_time is 0, philo 1 eats for 300
	cur_time is 300, philo 1 sleeps.
	cur_time is 300, philo 2 picks up fork.
	cur_time is 300, philo 2 starts eating.
	cur_time is 600, philo 2 sleeps.
*/

int	eat(int num, t_info *global, t_philo *philo)
{
	unsigned long	start;

	if (!check_status(global))
	{
		pthread_mutex_lock(&global->print_mutex);
		printf("\033[0;33m[%lu] %d is eating\n\033[0;30m", get_time() - global->start_time, num);
		pthread_mutex_unlock(&global->print_mutex);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	start = get_time();
	pthread_mutex_lock(&global->r_mutex);
	// if (start + philo->last_ate > (unsigned long)global->time_to_die)
	// if (start - philo->last_ate < global->time_to_die)
		philo->last_ate = start;
	pthread_mutex_unlock(&global->r_mutex);
	if (!wait_action(get_time(), global->time_to_eat, global))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	philo->num_eat++;
	return (1);
	// last ate should be updated after the philosopher is done eating, because the philosopher
	// might die while eating.
}

int	check_death(t_info *global, t_philo *philo)
{
	if (get_time() - philo->last_ate >= (unsigned long)global->time_to_die)
	{
		global->end_sim = 1;
		return (1);
	}
	return (0);
}

int	ft_sleep(int id, t_info *global)
{
	if (!check_status(global))
	{
		pthread_mutex_lock(&global->print_mutex);
		printf("\033[0;36m[%lu] %d is sleeping\n\033[0;30m", get_time() - global->start_time, id);
		pthread_mutex_unlock(&global->print_mutex);
	}
	else
		return (0);
	return (wait_action(get_time(), global->time_to_sleep, global));
}

void	*routine(void *d)
{
	t_philo	*philo;
	t_info	*global;

	philo = (t_philo *)d;
	global = philo->p_info;
	if (global->num_philo == 1)
		handle_one_philo(philo, global);
	if (philo->num % 2 == 0)
	{
		pthread_mutex_lock(&global->print_mutex);
		printf("\033[0;35m[%lu] %d is thinking\n\033[0;30m", get_time() - global->start_time, philo->num);
		pthread_mutex_unlock(&global->print_mutex);
		usleep(global->time_to_eat * 100);
	}
	while (!check_status(global))
	{
		// lock left fork
		pthread_mutex_lock(philo->left_fork);
		if (!check_status(global))
			pthread_mutex_lock(&global->print_mutex);
		else
		{
			pthread_mutex_unlock(philo->left_fork);
			break ;
		}
		printf("\033[0;32m[%lu] %d has taken left fork\n\033[0;30m", get_time() - global->start_time, philo->num);
		pthread_mutex_unlock(&global->print_mutex);
		// lock right fork
		pthread_mutex_lock(philo->right_fork);
		if (!check_status(global))
			pthread_mutex_lock(&global->print_mutex);
		else
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break ;
		}
		printf("\033[0;32m[%lu] %d has taken right fork\n\033[0;30m", get_time() - global->start_time, philo->num);
		pthread_mutex_unlock(&global->print_mutex);
		// start eating
		if (!eat(philo->num, global, philo))
			break ;
		// last ate should be the time they start eating
		// release forks
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		if (!ft_sleep(philo->num, global))
			break ;
		// should think after waking up if no 2 forks nearby
		if (!check_status(global))
		{
			pthread_mutex_lock(&global->print_mutex);
			printf("\033[0;35m[%lu] %d is thinking\n\033[0;30m", get_time() - global->start_time, philo->num);
			pthread_mutex_unlock(&global->print_mutex);
		}
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
		if (global->philo[i].num_eat >= *global->num_eat)
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

void	check_philos(t_info	*philos)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < philos->num_philo)
		{
			pthread_mutex_lock(&philos->r_mutex);
			if (get_time() - philos->philo[i].last_ate >= (unsigned long)philos->time_to_die)
			{
				pthread_mutex_lock(&philos->print_mutex);
				pthread_mutex_lock(&philos->d_mutex);
				philos->end_sim = 1;
				pthread_mutex_unlock(&philos->d_mutex);
				printf("[%lu] %d died\n", get_time() - philos->start_time, philos->philo[i].num);
				pthread_mutex_unlock(&philos->r_mutex);
				return ;
			}
			pthread_mutex_unlock(&philos->r_mutex);
		}
		if ((philos->num_eat && check_num_ate(philos)) || philos->end_sim)
			break ;
	}
	return ;
}

int	main(int ac, char **av)
{
	t_info	philos;
	int		i;

	validate_input(&philos, ac, av);
	philos.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * philos.num_philo);
	if (!philos.forks)
		exit(1);
	philos.philo = (t_philo *)malloc(sizeof(t_philo) * philos.num_philo);
	if (!philos.philo)
	{
		free(philos.forks);
		exit(1);
	}
	i = -1;
	while (++i < philos.num_philo)
		pthread_mutex_init(&philos.forks[i], 0);
	pthread_mutex_init(&philos.print_mutex, 0);
	pthread_mutex_init(&philos.r_mutex, 0);
	pthread_mutex_init(&philos.d_mutex, 0);
	i = -1;
	philos.start_time = get_time();
	pthread_mutex_lock(&philos.d_mutex);
	philos.end_sim = 0;
	pthread_mutex_unlock(&philos.d_mutex);
	while (++i < philos.num_philo)
	{
		philos.philo[i].num = i + 1;
		philos.philo[i].is_alive = 1;
		philos.philo[i].last_ate = philos.start_time;
		philos.philo[i].num_eat = 0;
		philos.philo[i].p_info = &philos;
		philos.philo[i].left_fork = &(philos.forks[i]);
		philos.philo[i].right_fork = &(philos.forks[(i + 1) % philos.num_philo]);
		pthread_create(&philos.philo[i].id, 0, routine, philos.philo + i);
		// usleep(200);
	}
	// parent process checks statuses of philos and stops simulation when a philo dies
	if (philos.num_philo > 1)
		check_philos(&philos);
	i = -1;
	while (++i < philos.num_philo)
		pthread_join(philos.philo[i].id, 0);
	finish_exec(&philos);
}

// 100 800 200 200