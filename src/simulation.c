/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 11:01:21 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/23 12:10:41 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/simulation.h"
#include "../includes/coder.h"
#include "../includes/dongle.h"
#include "../includes/monitor.h"
#include <pthread.h>

int	start_simulation(t_simulation *sim)
{
	int	i;

	sim->start_time_ms = get_time_ms();
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_create(
				&sim->coders[i].thread,
				NULL,
				coder_routine,
				&sim->coders[i]))
		{
			sim->config.number_of_coders = i;
			stop_simulation(sim);
			return (0);
		}
		i++;
	}
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim))
	{
		sim->config.number_of_coders = i;
		stop_simulation(sim);
		return (0);
	}
	return (1);
}

void	wait_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_join(sim->coders[i].thread, NULL) != 0)
			perror("pthread_join (coder)");
		i++;
	}
	if (pthread_join(sim->monitor_thread, NULL) != 0)
		perror("pthread_join (monitor)");
}

void	destroy_simulation(t_simulation *sim)
{
	int	i;

	if (!sim)
		return ;
	if (sim->coders)
	{
		i = 0;
		while (i < sim->initialized_dongles)
		{
			if (sim->dongles)
				pthread_mutex_destroy(&sim->dongles[i].mutex);
			i++;
		}
		free(sim->coders);
	}
	if (sim->dongles)
		free(sim->dongles);
	destroy_simulation_mutexes(sim);
}
