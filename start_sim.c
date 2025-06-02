/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_sim.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:40:46 by mel-bout          #+#    #+#             */
/*   Updated: 2025/06/02 23:22:50 by mel-bout         ###   ########.fr       */
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
	philo->lock = true;
	while (1)
	{
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
bool	stop(t_data *data)
{
	bool	sim;

	pthread_mutex_lock(&data->stop_tex);
	sim = data->stop_sim;
	pthread_mutex_unlock(&data->stop_tex);
	return (sim);
}
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
void	monitor(t_data *data)
{
	int	i;
	
	i = 0;
	while (1)
	{
		if (data->philo[i].lock == true)
		{
			pthread_mutex_lock(&data->stop_tex);
			if (curr_time(&data->philo[i]) - data->philo[i].last_meal > data->t_die 
				|| curr_time(&data->philo[i]) - data->philo[i].sleep > data->t_die)
			{
				// printf("je suis le monitor\n");
				data->stop_sim = true;
				printf("%ld %d died\n", curr_time(&data->philo[i]), data->philo[i].id);
				// atomic_eating(&data->philo[i], "died*******");
				pthread_mutex_unlock(&data->stop_tex);
				return	;
			}
			if (!stop_eating(data))
			{
				pthread_mutex_unlock(&data->stop_tex);
				return	;
			}
			pthread_mutex_unlock(&data->stop_tex);
		}
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
	monitor(data);
	// printf("fin de monitor\n");
	return (0);
}
