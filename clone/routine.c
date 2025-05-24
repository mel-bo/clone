/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:09:03 by mel-bout          #+#    #+#             */
/*   Updated: 2025/05/24 23:15:47 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	is_alive(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->stop_tex);
	if (philo->data->stop_sim == false
		&& get_time() - philo->eat >= philo->data->t_die)
	{
		philo->data->stop_sim = true;
		atomic_eating(philo, "died");
		pthread_mutex_unlock(&philo->data->stop_tex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->stop_tex);
	return (0);
}

void	is_eating(t_philo *philo, pthread_mutex_t *f1, pthread_mutex_t *f2)
{
	philo->eat = get_time();
	pthread_mutex_lock(f1);
	usleep(100);
	if (stop_sim(philo) == false)
		atomic_eating(philo, "has taken a fork");
		// printf("%ld [%d] has taken a fork\n", curr_time(philo), philo->id);
	while (philo->data->nb_philo == 1)
	{
		if (is_alive(philo))
		{
			pthread_mutex_unlock(f1);
			return ;
		}
		usleep(100);
	}
	pthread_mutex_lock(f2);
	if (stop_sim(philo) == false)
		atomic_eating(philo, "has taken a fork");
	is_alive(philo);
	if (stop_sim(philo) == false)
	{
		// printf("%ld [%d] is eating\n", curr_time(philo), philo->id);
		atomic_eating(philo, "is eating");
		while (get_time() - philo->eat < philo->data->t_eat)
			usleep(100);
	}
	pthread_mutex_unlock(f1);
	pthread_mutex_unlock(f2);
	nb_turn(philo);
}

void	eating(t_philo *philo)
{
	if (philo->id % 2 == 0)
		is_eating(philo, philo->r_fork, philo->l_fork);
	else
		is_eating(philo, philo->l_fork, philo->r_fork);
}

void	sleeping(t_philo *philo)
{
	atomic_eating(philo, "is sleeping");
	// printf("%ld [%d] is sleeping\n", curr_time(philo), philo->id);
	philo->sleep = get_time();
	while (get_time() - philo->sleep < philo->data->t_sleep)
		usleep(100);
}

void	thinking(t_philo *philo)
{
	atomic_eating(philo, "is thinking");
	// printf("%ld [%d] is thinking\n", curr_time(philo), philo->id);
	// usleep(100);
}
