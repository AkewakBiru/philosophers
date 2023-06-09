/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:24:53 by abiru             #+#    #+#             */
/*   Updated: 2023/03/30 12:39:04 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_strlen(const char *s)
{
	int	i;

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

static void	print_usage(void)
{
	write(2, "\033[0;31mUSAGE: ./philo \033[0;35mnumber_of_philosophers", 52);
	write(2, " \033[0;36mtime_to_die \033[0;34mtime_to_eat ", 39);
	write(2, "\033[0;32mtime_to_sleep \033[0;37m", 29);
	error_msg("number_of_times_a_philosopher_must_eat (optional)\033[0;37m");
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
		if (global->num_eat < 1 || global->num_eat > 2147483647)
			error_msg("Invalid");
	}
	else
		global->num_eat = 2147483649;
	if (global->num_philo < 1 || global->num_philo > 200
		|| global->time_to_die < 1 || global->time_to_eat < 1
		|| global->time_to_sleep < 1)
		error_msg("Invalid");
	if (av[5])
		global->num_eat++;
}
