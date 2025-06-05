/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_sim.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:40:46 by mel-bout          #+#    #+#             */
/*   Updated: 2025/06/05 14:31:07 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	nb_turn(t_philo *philo)
{
	if (philo->nb_sim > 0)
		philo->turn++;
	pthread_mutex_lock(&philo->data->st_eating);
	if (philo->nb_sim > 0 && philo->turn == philo->nb_sim)
		philo->stop_eat = true;
	pthread_mutex_unlock(&philo->data->st_eating);
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
	if (philo->t_start > 0)
		philo->lock = true;
	while (1)
	{
		if (philo->id % 2 == 0)
			usleep(100);
		if (stop_sim(philo) == false && philo->stop_eat == false)
			eating(philo);
		// printf("[%d] a fini de manger\n", philo->id);
		if (stop_sim(philo) == false && philo->stop_eat == false)
			sleeping(philo);
		// printf("[%d] a fini de dormir\n", philo->id);
		if (stop_sim(philo) == false && philo->stop_eat == false)
			thinking(philo);
		// printf("[%d] a fini de penser\n", philo->id);
		if (stop_sim(philo) == true || philo->stop_eat == true)
		{
			// printf("fin de la routine\n");
			return (NULL);
		}
		// printf("[%d] loop ?\n", philo->id);
	}
	return (NULL);
}
// bool	stop(t_data *data)
// {
// 	bool	sim;

// 	pthread_mutex_lock(&data->stop_tex);
// 	sim = data->stop_sim;
// 	pthread_mutex_unlock(&data->stop_tex);
// 	return (sim);
// }
int	stop_eating(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->st_eating);
	while (i < data->nb_philo)
	{
		if (data->philo[i].stop_eat == false)
		{
			pthread_mutex_unlock(&data->st_eating);
			return (1);
		}
		i++;
	}
	pthread_mutex_unlock(&data->st_eating);
	return (0);
}
int	monitor_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->stop_tex);
	if (philo->lock == true 
		&& curr_time(philo) - philo->last_meal > philo->data->t_die)
			return (1);
	pthread_mutex_unlock(&philo->data->stop_tex);
	return (0);
}

void	monitor(t_data *data)
{
	int	i;
	
	i = 0;
	while (1)
	{
		if (monitor_check(&data->philo[i]))
		{
			data->stop_sim = true;
			atomic_eating(&data->philo[i], "died");
			pthread_mutex_unlock(&data->stop_tex);
			return ;
		}
		if (!stop_eating(data))
			return ;
		i++;
		if (i == data->nb_philo)
			i = 0;
		usleep(10);
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
	if (data->nb_philo != 1)
		monitor(data);
	// printf("fin de monitor\n");
	return (0);
}
