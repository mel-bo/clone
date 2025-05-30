/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_sim.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dell <dell@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:40:46 by mel-bout          #+#    #+#             */
/*   Updated: 2025/05/30 19:31:17 by dell             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	nb_turn(t_philo *philo)
{
	if (philo->nb_sim > 0)
		philo->turn++;
	if (philo->nb_sim > 0 && philo->turn == philo->nb_sim)
		philo->stop_eat = true;
}

bool	stop_sim(t_philo *philo)
{
	bool	sim;

	pthread_mutex_lock(&philo->data->stop_tex);
	sim = philo->data->stop_sim;
	pthread_mutex_unlock(&philo->data->stop_tex);
	return (sim);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->t_start = get_time();
	// if (philo->id % 2 == 1)
	// 	usleep(10);
	while (1)
	{
		if (stop_sim(philo) == false && philo->stop_eat == false)
			eating(philo);
		if (stop_sim(philo) == false && philo->stop_eat == false)
			sleeping(philo);
		if (stop_sim(philo) == false && philo->stop_eat == false)
			thinking(philo);
		if (stop_sim(philo) == true || philo->stop_eat == true)
			return (NULL);
	}
	return (NULL);
}
bool	stop(t_data *data)
{
	bool	sim;

	pthread_mutex_lock(&data->stop_tex);
	sim = data->stop_sim;
	pthread_mutex_unlock(&data->stop_tex);
	return (sim);
}

void	monitor(t_data *data)
{
	int	i;
	
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&data->stop_tex);
		// printf("monitor [%d]\n", i);
		if (get_time() - data->philo[i].t_start - data->philo[i].eat > data->t_die)
		{
			data->stop_sim = true;
			atomic_eating(&data->philo[i], "died******");
			pthread_mutex_unlock(&data->stop_tex);
			return	;
		}
		pthread_mutex_unlock(&data->stop_tex);
		i++;
		if (i == data->nb_philo)
			i = 0;
		usleep(80);
	}
}

int	start_sim(t_data *data)
{
	int		i;
	int		t;
	t_philo	*philo;

	i = 0;
	philo = data->philo;
	while (i != data->nb_philo)
	{
		t = pthread_create(&philo[i].philo, NULL, routine, (void *)&philo[i]);
		if (t != 0)
		{
			printf("thread [%d] creation failed\n", i);
			sim_cleaning(data, i);
			return (1);
		}
		i++;
	}
	monitor(data);
	return (0);
}
