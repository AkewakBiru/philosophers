/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 07:54:49 by abiru             #+#    #+#             */
/*   Updated: 2023/03/19 22:54:39 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	validate_input(t_info *philo, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		exit(1);
	if (ft_atoi(av[1]) > 200 || ft_atoi(av[1]) < 1)
		exit(1);
	philo->num_philo = ft_atoi(av[1]);
	philo->time_to_eat = ft_atoi(av[2]);
	philo->time_to_die = ft_atoi(av[3]);
	philo->time_to_sleep = ft_atoi(av[4]);
	if (av[5] && ft_atoi(av[5]) > 0)
		philo->num_eat = ft_atoi(av[5]);
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

void	eat(int num, int time)
{
	printf("%d is eating\n", num);
	usleep(time);
}

int	check_death(t_info *new)
{
	// int	i;

	// i = 0;
	printf("ccchere\n");
	if (new->philo->is_alive == 0)
		return (0);
	// while (i < new->num_philo)
	// {
	// 	if (new->philo[i].is_alive == 0)
	// 		return (0);
	// 	i++;
	// }
	return (1);
}

void	ft_sleep(int id, long long int time)
{
	printf("%d is sleeping\n", id);
	usleep(time);
}

void *routine(void *d)
{
	// printf("here\n");
	t_philo *philo = (t_philo *)d;
	t_info *new = (t_info *)d;
	while (1)
	{
		if (philo->num % 2 == 0)
		{
			if (new->curr_time - philo->last_ate > new->time_to_die)
				philo->is_alive = 0;
			if (!philo->is_alive)
				break ;
			if (new->forks[philo->num].status == 0)
			{
				printf("%d has taken a fork\n", philo->num);
				pthread_mutex_lock(&new->forks[philo->num].fork);
				new->forks[philo->num].status = 1;
			}
			if (new->forks[philo->num + 1].status == 0)
			{
				printf("%d has taken a fork\n", philo->num);
				pthread_mutex_lock(&new->forks[philo->num + 1].fork);
				new->forks[philo->num + 1].status = 1;
			}
			if (new->forks[philo->num].status == 1 && new->forks[philo->num + 1].status == 1)
			{
				// set last ate and start eating
				new->philo->last_ate = new->curr_time;
				eat(philo->num, new->time_to_eat);
				// increment current time
				new->curr_time += new->time_to_eat;
				// release forks
				pthread_mutex_unlock(&new->forks[philo->num].fork);
				pthread_mutex_unlock(&new->forks[philo->num + 1].fork);
				ft_sleep(philo->num, new->time_to_sleep);
			}
			// check_death();
		}
		else
			printf("e\n");
	}
	// printf("%d has taken a fork\n", new->philo->num);
	// for (int i = 0; i<100000; i++)
	// {
	// 	// creates a lock so that another thread can't modify the content of a resource that is shared between threads.
	// 	pthread_mutex_lock(&lock);
	// 	// unlocks the lock so that another thread can modify a resource that is shared between the threads.
	// 	pthread_mutex_unlock(&lock);
	// }
	return (0);
}

int main(int ac, char **av)
{
	t_info	philos;
	int		i;

	validate_input(&philos, ac, av);
	philos.forks = (t_fork *)malloc(sizeof(t_fork) * philos.num_philo);
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
	philos.curr_time = philos.start_time;
	// printf("here\n");
	while (++i < philos.num_philo)
	{
		pthread_mutex_init(&philos.forks[i].fork, 0);
		philos.forks[i].status = 0;
	}
	// printf("here again\n");
	i = -1;
	while (++i < philos.num_philo)
	{
		philos.philo[i].num = i;
		philos.philo[i].is_alive = 1;
		philos.philo[i].last_ate = get_time();
		pthread_create(&philos.philo[i].id, 0, &routine, philos.philo + i);
	}
	// printf("here\n");
	i = -1;
	while (++i < philos.num_philo)
		pthread_join(philos.philo[i].id, 0);
	i = -1;
	while (++i < philos.num_philo)
		pthread_mutex_destroy(&philos.forks->fork + i);
	i = -1;
	while (++i < philos.num_philo)
	{
		pthread_detach(philos.philo[i].id);
		philos.philo[i].num = i;
	}
	free(philos.forks);
	free(philos.philo);
}
