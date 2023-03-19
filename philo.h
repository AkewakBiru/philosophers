/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 07:57:18 by abiru             #+#    #+#             */
/*   Updated: 2023/03/19 22:56:14 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "./libft/libft.h"

int ft_atoi(const char *str);
long long int	get_time();

typedef struct philo
{
	pthread_t id;
	int	num;
	// int	left_fork;
	// int right_fork;
	int	is_alive;
	int	last_ate;
}	t_philo;

typedef struct s_fork
{
	pthread_mutex_t fork;
	int	status;
}	t_fork;

typedef struct p_info
{
	int num_philo;
	int time_to_eat;
	int time_to_die;
	int	time_to_sleep;
	int	num_eat;
	t_fork	*forks;
	t_philo	*philo;
	int	start_time;
	int	curr_time;
}	t_info;

void	ft_putnbr_fd(int n, int fd);
void	ft_putendl_fd(char *s, int fd);
#endif
