/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 11:00:25 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/22 11:00:26 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H
# define SIMULATION_H

# include <pthread.h>
# include "parse_config.h"

typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;

typedef struct s_simulation
{
	t_config	config;
	t_coder		*coders;
	t_dongle	*dongles;

	pthread_mutex_t	log_mutex;
	long long	start_time_ms;
	pthread_t	monitor_thread;

	int		should_stop;
	pthread_mutex_t	stop_mutex;

	int		done_coders;

	/* number of dongle mutexes successfully initialized */
	int		initialized_dongles;

	pthread_mutex_t	sim_mutex;

	pthread_mutex_t	wait_mutex;
	pthread_cond_t	wait_cond;
} 	t_simulation;

int		init_simulation(t_simulation *sim, t_config *config);
int		start_simulation(t_simulation *sim);
void	wait_simulation(t_simulation *sim);
void	destroy_simulation(t_simulation *sim);

long long	get_sim_time(t_simulation *sim);
void	smart_sleep(t_simulation *sim, long long duration_ms);

int	simulation_stopped(t_simulation *sim);
void	stop_simulation(t_simulation *sim);

#endif
