/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 12:23:11 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/24 09:28:44 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/simulation.h"
#include "../includes/dongle.h"
#include "../includes/coder.h"

int	init_simulation(t_simulation *sim, t_config *config)
{
	if (!sim || !config)
		return (0);
	init_simulation_state(sim, config);
	if (!init_simulation_mutexes(sim))
		return (0);
	if (!alloc_and_init_resources(sim))
	{
		destroy_simulation_mutexes(sim);
		return (0);
	}
	return (1);
}

void	init_simulation_state(t_simulation *sim, t_config *config)
{
	sim->config = *config;
	sim->coders = NULL;
	sim->dongles = NULL;
	sim->should_stop = 0;
	sim->done_coders = 0;
	sim->initialized_dongles = 0;
}

int	init_simulation_mutexes(t_simulation *sim)
{
	if (pthread_mutex_init(&sim->sim_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
	{
		destroy_sim_mutexes(sim, 1);
		return (0);
	}
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
	{
		destroy_sim_mutexes(sim, 2);
		return (0);
	}
	return (1);
}

int	alloc_and_init_resources(t_simulation *sim)
{
	if (!alloc_simulation(sim))
		return (0);
	if (!init_dongles(sim))
	{
		free(sim->coders);
		free(sim->dongles);
		return (0);
	}
	init_coders(sim);
	return (1);
}
