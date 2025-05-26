/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 11:25:48 by mel-bout          #+#    #+#             */
/*   Updated: 2025/05/24 16:43:26 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/*
	faire en sorte que la moitier prend avec la droite
	et l'autre moitier avec la gauche
*/

void	mutex_destroy(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->fork[i].fork);
		i++;
	}
	pthread_mutex_destroy(&data->stop_tex);
	pthread_mutex_destroy(&data->print);
}

int	sim_cleaning(t_data *data, int n)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = data->philo;
	if (n == 0)
		n = data->nb_philo;
	while (i < n)
	{
		if (pthread_join(philo[i].philo, NULL) != 0)
			return (1);
		i++;
	}
	mutex_destroy(data);
	free(data->philo);
	free(data->fork);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (parsing(ac, av + 1))
		return (1);
	if (init_data(&data, av))
		return (1);
	if (start_sim(&data))
		return (1);
	if (sim_cleaning(&data, 0))
		return (1);
	return (0);
}
