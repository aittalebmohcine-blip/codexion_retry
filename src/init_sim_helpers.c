/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sim_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 12:23:52 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/24 09:28:40 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/simulation.h"
#include "../includes/coder.h"
#include "../includes/dongle.h"

void	destroy_sim_mutexes(t_simulation *sim, int stage)
{
	if (stage >= 4)
		pthread_mutex_destroy(&sim->coders_count_mutex);
	if (stage >= 3)
		pthread_mutex_destroy(&sim->log_mutex);
	if (stage >= 2)
		pthread_mutex_destroy(&sim->stop_mutex);
	if (stage >= 1)
		pthread_mutex_destroy(&sim->sim_mutex);
}

int	alloc_simulation(t_simulation *sim)
{
	int	n;

	n = sim->config.number_of_coders;
	sim->coders = malloc(sizeof(t_coder) * n);
	if (!sim->coders)
		return (0);
	sim->dongles = malloc(sizeof(t_dongle) * n);
	if (!sim->dongles)
	{
		free(sim->coders);
		return (0);
	}
	return (1);
}
