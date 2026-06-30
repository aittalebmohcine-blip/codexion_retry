/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 12:24:15 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/24 11:02:01 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/simulation.h"

void	destroy_simulation_mutexes(t_simulation *sim)
{
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->sim_mutex);
}

long long	get_sim_time(t_simulation *sim)
{
	return (get_time_ms() - sim->start_time_ms);
}

void	smart_sleep(t_simulation *sim, long long duration_ms)
{
	long long	start;

	start = get_time_ms();
	while (!simulation_stopped(sim))
	{
		if (get_time_ms() - start >= duration_ms)
			break ;
		usleep(500);
	}
}

int	simulation_stopped(t_simulation *sim)
{
	int	value;

	pthread_mutex_lock(&sim->stop_mutex);
	value = sim->should_stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (value);
}

void	stop_simulation(t_simulation *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->should_stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}
