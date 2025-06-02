/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 20:55:19 by mel-bout          #+#    #+#             */
/*   Updated: 2025/06/02 22:42:33 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

//	(*data).philos[philo, philo, philo]
//	(*data).philo + i	
//	&(*data).philo[i]

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	curr_time(t_philo *philo)
{
	return (get_time() - philo->t_start);
}

void	atomic_eating(t_philo *philo, char *str)
{
	// pthread_mutex_lock(&philo->data->print);
	if (stop_sim(philo) == false)
		printf("%ld %d %s\n", curr_time(philo), philo->id, str);
	// pthread_mutex_unlock(&philo->data->print);
}
