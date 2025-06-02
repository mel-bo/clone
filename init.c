/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:48:13 by mel-bout          #+#    #+#             */
/*   Updated: 2025/05/31 21:25:48 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	error_init_cleaning(t_data *data, int n, int j)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&data->fork[i].fork);
		i++;
	}
	if (j == 1 || j == 2)
		pthread_mutex_destroy(&data->stop_tex);
	if (j == 2)
		pthread_mutex_destroy(&data->print);
	free(data->philo);
	free(data->fork);
}

void	init_philo(t_data *data, char *arg)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (i < data->nb_philo)
	{
		data->philo[i].id = j;
		data->philo[i].turn = 0;
		data->philo[i].stop_eat = false;
		data->philo[i].lock = false;
		data->philo[i].data = data;
		data->philo[i].eat = 0;
		data->philo[i].last_meal = 0;
		data->philo[i].r_fork = &data->fork[i].fork;
		data->philo[i].l_fork = &data->fork[(i + 1) % data->nb_philo].fork;
		if (arg)
			data->philo[i].nb_sim = ft_atoi(arg);
		else
			data->philo[i].nb_sim = 0;
		i++;
		j++;
	}
}

int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->fork[i].fork, NULL) != 0)
		{
			printf("Mutex %d init failed\n", i);
			return (error_init_cleaning(data, i, 0), 1);
		}
		i++;
	}
	if (pthread_mutex_init(&data->stop_tex, NULL) != 0)
	{
		printf("Mutex STOP_TEX init failed\n");
		return (error_init_cleaning(data, 1, 1), 1);
	}
	if (pthread_mutex_init(&data->print, NULL) != 0)
	{
		printf("Mutex print init failed\n");
		return (error_init_cleaning(data, 1, 2), 1);
	}
	pthread_mutex_init(&data->st_eating, NULL);
	return (0);
}

int	init_data(t_data *data, char **arg)
{
	data->nb_philo = ft_atoi(arg[1]);
	data->t_die = ft_atoi(arg[2]);
	data->t_eat = ft_atoi(arg[3]);
	data->t_sleep = ft_atoi(arg[4]);
	data->stop_sim = false;
	data->philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!(data->philo))
		return (1);
	data->fork = malloc(sizeof(t_fork) * data->nb_philo);
	if (!(data->fork))
		return (free(data->philo), 1);
	init_mutex(data);
	init_philo(data, arg[5]);
	return (0);
}
