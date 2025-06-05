/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:43:46 by mel-bout          #+#    #+#             */
/*   Updated: 2025/06/05 12:21:23 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	error_msg(int n)
{
	if (n == 1)
		printf("Error incorrect number of arguments\n");
	else if (n == 2)
		printf("Error incorrect type of arguments\n");
	else if (n == 3)
		printf("Error arguments must be grater than 0\n");
	printf("Correct arguments :\n");
	printf("[number of philo] [time to die] [time to eat] ");
	printf("[time to sleep] [number of time to eat](optional)\n");
}

int	parsing(int ac, char **av)
{
	int	i;

	i = 0;
	if (ac != 5 && ac != 6)
		return (error_msg(1), 1);
	while (av[i])
	{
		if (ft_isdigit(av[i]))
			return (error_msg(2), 1);
		i++;
	}
	i = 0;
	while (i < 4)
	{
		if (ft_atoi(av[i]) == 0)
			return (error_msg(3), 1);
		i++;
	}
	return (0);
}
