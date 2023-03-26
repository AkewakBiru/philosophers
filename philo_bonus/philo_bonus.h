#ifndef PHILO_BONUS_H
#define PHILO_BONUS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>

struct	s_info;

typedef struct philo
{
	pthread_t		id;
	int				num;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
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
	t_philo			*philo;
	unsigned long	start_time;
}	t_info;

void	validate_input(t_info *global, int ac, char **av);
int		ft_atoi(const char *str);
#endif