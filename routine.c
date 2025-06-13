/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:09:03 by mel-bout          #+#    #+#             */
/*   Updated: 2025/06/05 19:33:20 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

// int	is_alive(t_philo *philo)
// {
// 	// printf("is alive [%d]\n", philo->id);
// 	pthread_mutex_lock(&philo->data->stop_tex);
// 	if (philo->data->stop_sim == false
// 		&& curr_time(philo) - philo->last_meal >= philo->data->t_die)
// 	{
// 		philo->data->stop_sim = true;
// 		atomic_eating(philo, "died");
// 		pthread_mutex_unlock(&philo->data->stop_tex);
// 		return (1);
// 	}
// 	pthread_mutex_unlock(&philo->data->stop_tex);
// 	return (0);
// }
void	last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->stop_tex);
	philo->last_meal = curr_time(philo);
	pthread_mutex_unlock(&philo->data->stop_tex);
}

void	one_philo(t_philo *philo, pthread_mutex_t *f1)
{
	while (curr_time(philo) < philo->data->t_die)
		usleep(100);
	printf("%ld %d died\n", curr_time(philo), philo->id);
	philo->data->stop_sim = true;
	pthread_mutex_unlock(f1);
	return ;
}

void	is_eating(t_philo *philo, pthread_mutex_t *f1, pthread_mutex_t *f2)
{
	pthread_mutex_lock(f1);
	if (stop_sim(philo) == false)
		atomic_eating(philo, "has taken a fork");
	if (philo->data->nb_philo == 1)
		return (one_philo(philo, f1));
	pthread_mutex_lock(f2);
	if (stop_sim(philo) == false)
		atomic_eating(philo, "has taken a fork");
	if (stop_sim(philo) == false)
	{
		philo->eat = get_time();
		atomic_eating(philo, "is eating");
		while (get_time() - philo->eat < philo->data->t_eat)
			usleep(100);
	}
	last_meal(philo);
	pthread_mutex_unlock(f1);
	pthread_mutex_unlock(f2);
	nb_turn(philo);
}

void	eating(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		is_eating(philo, philo->r_fork, philo->l_fork);
	}
	else
	{
		is_eating(philo, philo->l_fork, philo->r_fork);
	}
}

void	sleeping(t_philo *philo)
{
	atomic_eating(philo, "is sleeping");
	philo->sleep = get_time();
	while (get_time() - philo->sleep < philo->data->t_sleep)
		usleep(100);
}

void	thinking(t_philo *philo)
{
	atomic_eating(philo, "is thinking");
	// if (philo->data->nb_philo % 2 == 1 && philo->id % 2 == 1)
	// 	usleep(10);
}
