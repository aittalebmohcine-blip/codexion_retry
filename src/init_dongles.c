/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 09:33:45 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/23 09:33:47 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "dongle.h"
#include "simulation.h"

static void	init_dongle(t_dongle *dongle)
{
	dongle->waiters.size = 0;
	dongle->available = 1;
	dongle->next_available_time_ms = 0;
}

static void	destroy_initialized_dongles(t_dongle *dongles, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		pthread_mutex_destroy(&dongles[j].mutex);
		j++;
	}
}

int	init_dongles(t_simulation *sim)
{
	int	i;

	if (!sim || !sim->dongles || sim->config.number_of_coders <= 0)
		return (0);
	sim->initialized_dongles = 0;
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
		{
			destroy_initialized_dongles(sim->dongles, i);
			sim->initialized_dongles = 0;
			return (0);
		}
		init_dongle(&sim->dongles[i]);
		i++;
		sim->initialized_dongles++;
	}
	return (1);
}
