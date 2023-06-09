/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:44:12 by abiru             #+#    #+#             */
/*   Updated: 2023/03/31 11:29:49 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	error_msg(char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
}

static void	check_alpha(int ac, char **av)
{
	int	i;
	int	j;

	i = 0;
	while (++i < ac)
	{
		j = 0;
		while (av[i][j] && (av[i][j] == ' ' || av[i][j] == '\t'
			|| av[i][j] == '\r' || av[i][j] == '\n'
			|| av[i][j] == '\v' || av[i][j] == '\f'))
			j++;
		if (av[i][j] == '+')
			j++;
		while (av[i][j] && av[i][j] >= '0' && av[i][j] <= '9')
				j++;
		while (av[i][j] && (av[i][j] == ' ' || av[i][j] == '\t'))
			j++;
		if (av[i][j] != '\0')
			error_msg("Invalid");
	}
}

void	validate_input(t_info *global, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		print_usage();
	check_alpha(ac, av);
	if (ft_atoi(av[2]) == -1 || ft_atoi(av[3]) == -1 || ft_atoi(av[4]) == -1)
		error_msg("Invalid");
	global->num_philo = ft_atoi(av[1]);
	global->time_to_die = ft_atoi(av[2]);
	global->time_to_eat = ft_atoi(av[3]);
	global->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
	{
		global->num_eat = ft_atoi(av[5]);
		if (global->num_eat < 1)
			error_msg("Invalid");
	}
	else
		global->num_eat = -1;
	if (global->num_philo < 1 || global->num_philo > 200
		|| global->time_to_die < 1 || global->time_to_eat < 1
		|| global->time_to_sleep < 1)
		error_msg("Invalid");
}

void	wait_threads(t_info *philos)
{
	int	i;

	i = -1;
	while (++i < philos->num_philo)
	{
		if (pthread_join(philos->philo[i].id, 0) != 0)
		{
			free(philos->forks);
			free(philos->f);
			free(philos->philo);
			error_msg("Wait failed");
		}
	}
}
