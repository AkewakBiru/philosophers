/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 07:57:18 by abiru             #+#    #+#             */
/*   Updated: 2023/03/21 21:43:48 by abiru            ###   ########.fr       */
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

struct	s_info;

int ft_atoi(const char *str);
unsigned long	get_time();

typedef struct s_fork
{
	pthread_mutex_t fork;
	int	status;
}	t_fork;

typedef struct philo
{
	pthread_t id;
	int	num;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t *right_fork;
	int	is_alive;
	unsigned long	last_ate;
	int num_eat;
	struct s_info *p_info;
}	t_philo;

typedef struct s_info
{
	int num_philo;
	int time_to_eat;
	int time_to_die;
	int	time_to_sleep;
	int	*num_eat;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
	t_philo	*philo;
	unsigned long	start_time;
}	t_info;

void	ft_putnbr_fd(int n, int fd);
void	ft_putendl_fd(char *s, int fd);
#endif
