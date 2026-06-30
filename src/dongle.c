/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 11:00:58 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/24 09:33:58 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/dongle.h"
#include "../includes/coder.h"
#include "../includes/simulation.h"

static int	wait_for_dongles(t_coder *coder);
static void	acquire_dongles(t_coder *coder);

int	take_dongles(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
		return (0);
	add_request(coder);
	if (!wait_for_dongles(coder))
		return (0);
	acquire_dongles(coder);
	unlock_dongles(coder);
	return (1);
}

void	release_dongles(t_coder *coder)
{
	long long	now;

	if (!coder->has_dongles)
		return ;
	now = get_sim_time(coder->sim);
	lock_dongles(coder);
	heap_remove_request(&coder->left_dongle->waiters, &coder->request);
	heap_remove_request(&coder->right_dongle->waiters, &coder->request);
	coder->requested = 0;
	coder->left_dongle->available = 1;
	coder->left_dongle->next_available_time_ms = now
		+ coder->sim->config.dongle_cooldown;
	coder->right_dongle->available = 1;
	coder->right_dongle->next_available_time_ms = now
		+ coder->sim->config.dongle_cooldown;
	coder->has_dongles = 0;
	unlock_dongles(coder);
}

static int	wait_for_dongles(t_coder *coder)
{
	while (!simulation_stopped(coder->sim))
	{
		lock_dongles(coder);
		if (can_take(coder))
			return (1);
		unlock_dongles(coder);
		usleep(1000);
	}
	return (0);
}

static void	acquire_dongles(t_coder *coder)
{
	coder->left_dongle->available = 0;
	coder->right_dongle->available = 0;
	coder->has_dongles = 1;
	log_action(coder, "has taken a dongle");
	log_action(coder, "has taken a dongle");
}
