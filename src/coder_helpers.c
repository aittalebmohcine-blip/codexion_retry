/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 21:05:23 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/30 21:05:25 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/coder.h"
#include "../includes/simulation.h"

void	handle_even_coder_sleep(t_coder *coder)
{
	long long	sleep_delay;

	sleep_delay = coder->sim->config.time_to_compile
		+ coder->sim->config.dongle_cooldown;
	smart_sleep(coder->sim, sleep_delay / 4);
}

void	state_and_mark_as_done(t_coder *coder)
{
	set_state(coder, STATE_DONE);
	mark_coder_done(coder);
}
