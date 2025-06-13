/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 11:27:42 by mel-bout          #+#    #+#             */
/*   Updated: 2025/06/07 21:22:49 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_fork
{
	pthread_mutex_t	fork;
}	t_fork;

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				turn;
	int				nb_sim;
	int				last_meal;
	long			t_start;
	long			eat;
	long			sleep;
	bool			stop_eat;
	int				lock;
	pthread_t		philo;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	bool			stop_sim;
	pthread_t		monitor;
	pthread_mutex_t	stop_tex;
	pthread_mutex_t	print;
	pthread_mutex_t	st_eating;
	pthread_mutex_t	lock;
	t_philo			*philo;
	t_fork			*fork;
}	t_data;

//-------------UTILS--------------
int		ft_atoi(const char *str);
void	error_msg(int n);
long	get_time(void);
long	curr_time(t_philo *philo);
int		sim_cleaning(t_data *data, int n);
void	atomic_eating(t_philo *philo, char *str);

//------------PARSING-------------
int		parsing(int ac, char **av);
int		ft_isdigit(char	*arg);

//--------------INIT--------------
int		init_data(t_data *data, char **arg);

//-----------START_SIM------------
int		start_sim(t_data *data);
void	*routine(void *arg);
void	nb_turn(t_philo *philo);
bool	stop_sim(t_philo *philo);

//-------------ROUTINE-------------
void	eating(t_philo *philo);
void	sleeping(t_philo *philo);
void	thinking(t_philo *philo);

#endif