/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 11:01:10 by mait-tal          #+#    #+#             */
/*   Updated: 2026/07/01 19:11:55 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/monitor.h"
#include "../includes/simulation.h"
#include "../includes/coder.h"

static int	check_burnouts(t_simulation *sim);
static int	all_coders_done(t_simulation *sim);

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (!simulation_stopped(sim))
	{
		if (check_burnouts(sim))
			return (NULL);
		if (all_coders_done(sim))
		{
			stop_simulation(sim);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

static int	all_coders_done(t_simulation *sim)
{
	int	done;

	pthread_mutex_lock(&sim->sim_mutex);
	done = (sim->done_coders
			== sim->config.number_of_coders);
	pthread_mutex_unlock(&sim->sim_mutex);
	return (done);
}

static int	check_burnouts(t_simulation *sim)
{
	int			i;
	int			state;
	long long	last_compile_time_ms;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->sim_mutex);
		state = sim->coders[i].state;
		last_compile_time_ms = sim->coders[i].last_compile_time_ms;
		pthread_mutex_unlock(&sim->sim_mutex);
		if (state != STATE_DONE
			&& get_sim_time(sim)
			- last_compile_time_ms
			> sim->config.time_to_burnout)
		{
			stop_simulation(sim);
			set_state(&sim->coders[i], STATE_BURNED_OUT);
			log_action(&sim->coders[i], "burned out");
			return (1);
		}
		i++;
	}
	return (0);
}
