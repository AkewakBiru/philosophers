/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:55:04 by abiru             #+#    #+#             */
/*   Updated: 2023/03/28 22:33:49 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <semaphore.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <pthread.h>
# define ALIVE 1
# define DEAD 0
# define COL_FORK "\033[0;32m"
# define COL_EAT "\033[0;33m"
# define COL_THINK "\033[0;35m"
# define COL_SLEEP "\033[0;36m"
# define COL_DFL "\033[0;37m"

typedef struct s_info
{
	int				id;
	sem_t			*sem_d;
	sem_t			*sem_chk;
	sem_t			*sem_p;
	sem_t			*sem_f;
	int				num_philo;
	unsigned long	last_ate;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				status;
	unsigned int	num_eat;
	unsigned int	eat_count;
	unsigned long	start_time;
}	t_info;

void			validate_input(t_info *global, int ac, char **av);
int				ft_atoi(const char *str);
unsigned long	get_time(void);
int				check_death(t_info *global);
int				wait_action(unsigned long start, unsigned long time,
					t_info *global);
void			unlink_semaphores(void);
void			close_semaphores(t_info *philos);
void			open_semaphores(t_info *philos);
void			init_procs(t_info *philos, int proc[]);
void			routine(t_info *philos);
void			eat(t_info *philos);
void			get_forks(t_info *philos);
void			ft_sleep(t_info *philos);
#endif