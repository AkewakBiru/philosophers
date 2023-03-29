/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 19:48:32 by abiru             #+#    #+#             */
/*   Updated: 2023/03/29 11:26:22 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_usage(void)
{
	write(2, "\033[0;31mUSAGE: ./philo \033[0;35mnumber_of_philosophers", 52);
	write(2, " \033[0;36mtime_to_die \033[0;34mtime_to_eat ", 39);
	write(2, "\033[0;32mtime_to_sleep \033[0;37m", 29);
	error_msg("number_of_times_a_philosopher_must_eat (optional)\033[0;37m");
}

static int	skip_spaces(const char *str)
{
	static int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	return (i);
}

static int	cus_strlen(const char *str)
{
	int	i;
	int	j;

	j = 0;
	i = skip_spaces(str);
	while (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] == '0')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
			j++;
		else
			break ;
		i++;
	}
	return (j);
}

static int	ft_sign(const char *str, int i)
{
	if (str[i] == '-')
		return (-1);
	return (1);
}

int	ft_atoi(const char *str)
{
	unsigned long long	result;
	int					i;

	result = 0;
	i = (int)skip_spaces(str);
	if ((int)ft_sign(str, i) == -1)
		return (-1);
	if (str[i] == '+')
		i++;
	if (cus_strlen(str) >= 11)
		return (-1);
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		if (result > 2147483647)
			return (-1);
		result = (result * 10) + str[i] - '0';
		i++;
	}
	if (result > 2147483647)
		return (-1);
	return (result);
}
