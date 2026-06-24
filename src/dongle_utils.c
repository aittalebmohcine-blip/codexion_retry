/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 09:34:01 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/24 09:28:19 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/dongle.h"
#include "../includes/coder.h"
#include "../includes/simulation.h"

void	lock_dongles(t_coder *coder)
{
	if (coder->left_dongle < coder->right_dongle)
	{
		pthread_mutex_lock(&coder->left_dongle->mutex);
		pthread_mutex_lock(&coder->right_dongle->mutex);
	}
	else
	{
		pthread_mutex_lock(&coder->right_dongle->mutex);
		pthread_mutex_lock(&coder->left_dongle->mutex);
	}
}

void	unlock_dongles(t_coder *coder)
{
	if (coder->left_dongle < coder->right_dongle)
	{
		pthread_mutex_unlock(&coder->right_dongle->mutex);
		pthread_mutex_unlock(&coder->left_dongle->mutex);
	}
	else
	{
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		pthread_mutex_unlock(&coder->right_dongle->mutex);
	}
}

int	can_take(t_coder *coder)
{
	long long	now;

	now = get_sim_time(coder->sim);
	if (heap_top(&coder->left_dongle->waiters) != &coder->request)
		return (0);
	if (heap_top(&coder->right_dongle->waiters) != &coder->request)
		return (0);
	if (!coder->left_dongle->available
		|| now < coder->left_dongle->next_available_time_ms
		|| !coder->right_dongle->available
		|| now < coder->right_dongle->next_available_time_ms)
		return (0);
	return (1);
}

void	add_request(t_coder *coder)
{
	long long	deadline;

	lock_dongles(coder);
	if (coder->sim->config.scheduler_type == SCHEDULER_FIFO)
		coder->request.priority = get_sim_time(coder->sim);
	else
	{
		pthread_mutex_lock(&coder->sim->sim_mutex);
		deadline = coder->last_compile_time_ms;
		pthread_mutex_unlock(&coder->sim->sim_mutex);
		coder->request.priority = deadline
			+ coder->sim->config.time_to_burnout;
	}
	if (!coder->requested)
	{
		heap_insert(&coder->left_dongle->waiters,
			&coder->request);
		heap_insert(&coder->right_dongle->waiters,
			&coder->request);
		coder->requested = 1;
	}
	unlock_dongles(coder);
}
