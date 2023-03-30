/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 07:57:18 by abiru             #+#    #+#             */
/*   Updated: 2023/03/30 23:06:43 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

struct	s_info;

typedef struct philo
{
	pthread_t		id;
	int				num;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				*l_val;
	int				*r_val;
	int				is_alive;
	unsigned long	last_ate;
	int				num_eat;
	struct s_info	*p_info;
}	t_philo;

typedef struct s_info
{
	int				num_philo;
	unsigned int	time_to_eat;
	unsigned int	time_to_die;
	unsigned int	time_to_sleep;
	int				end_sim;
	int				num_eat;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	r_mutex;
	pthread_mutex_t	d_mutex;
	pthread_mutex_t	*forks;
	int				*f;
	t_philo			*philo;
	unsigned long	start_time;
}	t_info;

int				ft_atoi(const char *str);
unsigned long	get_time(void);
void			validate_input(t_info *global, int ac, char **av);
void			error_msg(char *msg);
int				init_mutexes(t_info *philos);
int				init_philos(t_info *philos);
void			init_ints(t_info *philos);
void			alloc_mem(t_info *philos);
void			*routine(void *d);
int				eat(int num, t_info *global, t_philo *philo);
int				ft_sleep(int id, t_info *global);
int				think(t_info *global, t_philo *philo);
int				lock_left_fork(t_info *global, t_philo *philo);
int				lock_right_fork(t_info *global, t_philo *philo);
int				check_status(t_info *global);
int				wait_action(unsigned long start, unsigned long time,
					t_info *global);
void			finish_exec(t_info *philos);
void			single_philo(t_info *philos);
void			*handle_one_philo(void *d);
int				handle_odd_philos(t_info *global, t_philo *philo);
int				print_death(t_info *philos, int i);
void			unlock_forks(t_philo *philo);
void			wait_threads(t_info *philos);
void			print_usage(void);
void			wait_left_fork(t_philo *philo);
int				terminate_threads(t_info *philos, int i);
#endif
