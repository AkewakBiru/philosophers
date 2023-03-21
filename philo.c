/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 07:54:49 by abiru             #+#    #+#             */
/*   Updated: 2023/03/21 22:00:19 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	validate_input(t_info *global, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		exit(1);
	if (ft_atoi(av[1]) > 200 || ft_atoi(av[1]) < 1)
		exit(1);
	global->num_philo = ft_atoi(av[1]);
	global->time_to_eat = ft_atoi(av[2]);
	global->time_to_die = ft_atoi(av[3]);
	global->time_to_sleep = ft_atoi(av[4]);
	if (av[5] && ft_atoi(av[5]) > 0)
		*global->num_eat = ft_atoi(av[5]);
	else
		global->num_eat = 0;
}

void	finish_exec(t_info *philos)
{
	int	i;

	i = -1;
	while (++i < philos->num_philo)
		pthread_mutex_destroy(philos->forks + (sizeof(pthread_mutex_t) * i));
	pthread_mutex_destroy(&philos->print_mutex);
	i = -1;
	while (++i < philos->num_philo)
	{
		pthread_detach(philos->philo[i].id);
		philos->philo[i].num = i;
	}
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

void	eat(int num, t_info *new)
{
	pthread_mutex_lock(&new->print_mutex);
	printf("[%lu] %d is eating\n", get_time() - new->start_time, num);
	pthread_mutex_unlock(&new->print_mutex);
	usleep(new->time_to_eat * 1000);
}

int	check_death(t_philo *philo)
{
	if (get_time() - (unsigned long)philo->last_ate > (unsigned long)philo->p_info->time_to_die)
		return (1);
	return (0);
}

void	ft_sleep(int id, t_info *global)
{
	printf("[%lu] %d is sleeping\n", get_time() - global->start_time, id);
	usleep(global->time_to_sleep * 1000);
}

void *routine(void *d)
{
	t_philo *philo = (t_philo *)d;
	t_info *global = philo->p_info;
	if (philo->num % 2 == 0)
	{
		pthread_mutex_lock(&global->print_mutex);
		printf("[%lu] %d is thinking\n", get_time() - global->start_time, philo->num);
		pthread_mutex_unlock(&global->print_mutex);
		usleep(global->time_to_eat / 4);
	}
	while (1)
	{
		// lock left fork
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&global->print_mutex);
		printf("[%lu] %d has taken left fork\n", get_time() - global->start_time, philo->num);
		pthread_mutex_unlock(&global->print_mutex);
		// lock right fork
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&global->print_mutex);
		printf("[%lu] %d has taken right fork\n", get_time() - global->start_time, philo->num);
		pthread_mutex_unlock(&global->print_mutex);
		philo->last_ate = get_time();
		philo->num_eat++;
		// start eating
		eat(philo->num, global);
		// release forks
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		ft_sleep(philo->num, global);
		// should think after waking up
		pthread_mutex_lock(&global->print_mutex);
		printf("[%lu] %d is thinking\n", get_time() - global->start_time, philo->num);
		pthread_mutex_unlock(&global->print_mutex);
	}
	// printf("here\n");
	return (0);
}

void	check_philos(t_info	*philos)
{
	(void)philos;
	return ;
}

int main(int ac, char **av)
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
	philos.start_time = get_time();
	while (++i < philos.num_philo)
		pthread_mutex_init(&philos.forks[i], 0);
	pthread_mutex_init(&philos.print_mutex, 0);
	i = -1;
	while (++i < philos.num_philo)
	{
		philos.philo[i].num = i;
		philos.philo[i].is_alive = 1;
		philos.philo[i].last_ate = get_time();
		philos.philo[i].num_eat = 0;
		philos.philo[i].p_info = &philos;
		philos.philo[i].left_fork = &(philos.forks[i]);
		philos.philo[i].right_fork = &(philos.forks[(i + 1) % philos.num_philo]);
		pthread_create(&philos.philo[i].id, 0, &routine, philos.philo + i);
	}
	// parent process checks statuses of philos and stops simulation when a philo dies
	check_philos(&philos);
	// pthread_mutex_lock(&philos.print_mutex);
	// printf("[%lu] %d died\n", get_time() - philos.start_time, philos[]);
	// pthread_mutex_unlock(&global->print_mutex);
	i = -1;
	while (++i < philos.num_philo)
		pthread_join(philos.philo[i].id, 0);
	finish_exec(&philos);
}
