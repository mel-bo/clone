/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:48:13 by mel-bout          #+#    #+#             */
/*   Updated: 2025/06/07 21:28:07 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	error_init_cleanup(t_data *data, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&data->fork[i].fork);
		i++;
	}
	free(data->philo);
	free(data->fork);
}

int	cleanup(t_data *data)
{
	error_init_cleanup(data, data->nb_philo);
	return (0);
}

int	cleanup2(t_data *data)
{
	error_init_cleanup(data, data->nb_philo);
	pthread_mutex_destroy(&data->stop_tex);
	return (0);
}

int	cleanup3(t_data *data)
{
	error_init_cleanup(data, data->nb_philo);
	pthread_mutex_destroy(&data->stop_tex);
	pthread_mutex_destroy(&data->print);
	return (0);
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
		data->philo[i].lock = 0;
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

int	safe_mutex(pthread_mutex_t *m, char *s, int (*cleanup)(t_data *), t_data *d)
{
	if (pthread_mutex_init(m, NULL) != 0)
	{
		printf("Mutex %s init failed\n", s);
		cleanup(d);
		return (1);
	}
	return (0);
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
			return (error_init_cleanup(data, i), 1);
		}
		i++;
	}
	safe_mutex(&data->stop_tex, "STOP_TEX", cleanup, data);
	safe_mutex(&data->print, "PRINT", cleanup2, data);
	safe_mutex(&data->st_eating, "ST_EATING", cleanup3, data);
	pthread_mutex_init(&data->lock, NULL);
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
