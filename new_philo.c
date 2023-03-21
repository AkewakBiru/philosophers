#include "philo.h"

void	validate_input(t_data *philo, int ac, char **av)
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

int main(int ac, char **av)
{
	t_fork	*forks;
	t_data	*data;
	int		i;

	validate_input(&data, ac, av);
}