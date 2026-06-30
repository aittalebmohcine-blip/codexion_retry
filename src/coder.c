/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 11:00:50 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/24 09:27:42 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/coder.h"
#include "../includes/simulation.h"
#include "../includes/dongle.h"

void	init_coders(t_simulation *sim)
{
	int	i;
	int	n;

	if (!sim || !sim->coders || !sim->dongles)
		return ;
	n = sim->config.number_of_coders;
	if (n <= 0)
		return ;
	memset(sim->coders, 0, sizeof(sim->coders[0]) * n);
	i = 0;
	while (i < n)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		sim->coders[i].state = STATE_IDLE;
		sim->coders[i].last_compile_time_ms = 0;
		sim->coders[i].compiles_done = 0;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % n];
		sim->coders[i].has_dongles = 0;
		sim->coders[i].request.coder = &sim->coders[i];
		sim->coders[i].request.priority = 0;
		sim->coders[i].requested = 0;
		i++;
	}
}

int	compile(t_coder *coder)
{
	if (simulation_stopped(coder->sim))
		return (0);
	pthread_mutex_lock(&coder->sim->sim_mutex);
	coder->last_compile_time_ms = get_sim_time(coder->sim);
	pthread_mutex_unlock(&coder->sim->sim_mutex);
	set_state(coder, STATE_COMPILING);
	smart_sleep(coder->sim, coder->sim->config.time_to_compile);
	if (simulation_stopped(coder->sim))
		return (0);
	coder->compiles_done++;
	if (coder_is_done(coder))
	{
		set_state(coder, STATE_DONE);
		mark_coder_done(coder);
		return (0);
	}
	return (1);
}

void	debug(t_coder *coder)
{
	if (simulation_stopped(coder->sim))
		return ;
	set_state(coder, STATE_DEBUGGING);
	smart_sleep(coder->sim, coder->sim->config.time_to_debug);
}

void	refactor(t_coder *coder)
{
	if (simulation_stopped(coder->sim))
		return ;
	set_state(coder, STATE_REFACTORING);
	smart_sleep(coder->sim, coder->sim->config.time_to_refactor);
}

void	wait_for_start(t_simulation *sim)
{
	pthread_mutex_lock(&sim->coders_count_mutex);
	while (!sim->created_coders_count)
		pthread_cond_wait(&sim->barriere,
			&sim->coders_count_mutex);
	pthread_mutex_unlock(&sim->coders_count_mutex);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	wait_for_start(coder->sim);
	while (!simulation_stopped(coder->sim))
	{
		if (!take_dongles(coder))
			break ;
		if (!compile(coder))
		{
			release_dongles(coder);
			break ;
		}
		release_dongles(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
