/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 09:00:08 by mait-tal          #+#    #+#             */
/*   Updated: 2026/07/01 19:10:12 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/coder.h"
#include "../includes/simulation.h"

int	coder_is_done(t_coder *coder)
{
	if (coder->sim->config.number_of_compiles_required <= 0)
		return (1);
	return (coder->compiles_done
		>= coder->sim->config.number_of_compiles_required);
}

void	mark_coder_done(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->sim_mutex);
	coder->sim->done_coders++;
	pthread_mutex_unlock(&coder->sim->sim_mutex);
}

void	set_state(t_coder *coder, t_coder_state state)
{
	pthread_mutex_lock(&coder->sim->sim_mutex);
	coder->state = state;
	pthread_mutex_unlock(&coder->sim->sim_mutex);
	if (state == STATE_COMPILING)
		log_action(coder, "is compiling");
	else if (state == STATE_DEBUGGING)
		log_action(coder, "is debugging");
	else if (state == STATE_REFACTORING)
		log_action(coder, "is refactoring");
}

void	log_action(t_coder *coder, char *msg)
{
	pthread_mutex_lock(&coder->sim->log_mutex);
	if (!simulation_stopped(coder->sim) || !strcmp(msg, "burned out"))
		printf("%lld %d %s\n", get_sim_time(coder->sim), coder->id, msg);
	pthread_mutex_unlock(&coder->sim->log_mutex);
}

void	wait_for_start(t_simulation *sim)
{
	pthread_mutex_lock(&sim->coders_count_mutex);
	while (!sim->created_coders_count)
		pthread_cond_wait(&sim->barriere,
			&sim->coders_count_mutex);
	pthread_mutex_unlock(&sim->coders_count_mutex);
}
