/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 11:01:21 by mait-tal          #+#    #+#             */
/*   Updated: 2026/07/01 19:12:42 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/simulation.h"
#include "../includes/coder.h"
#include "../includes/dongle.h"
#include "../includes/monitor.h"

static void	start_all_threads(t_simulation *sim)
{
	pthread_mutex_lock(&sim->coders_count_mutex);
	sim->start_time_ms = get_time_ms();
	sim->created_coders_count = 1;
	pthread_cond_broadcast(&sim->barriere);
	pthread_mutex_unlock(&sim->coders_count_mutex);
}

int	start_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_create(
				&sim->coders[i].thread,
				NULL,
				coder_routine,
				&sim->coders[i]))
		{
			stop_simulation(sim);
			start_all_threads(sim);
			while (i > 0)
			{
				i--;
				pthread_join(sim->coders[i].thread, NULL);
			}
			destroy_simulation(sim);
			return (0);
		}
		i++;
	}
	start_all_threads(sim);
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim))
	{
		stop_simulation(sim);
		while (i > 0)
			{
				i--;
				pthread_join(sim->coders[i].thread, NULL);
			}
		destroy_simulation(sim);
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
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
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
