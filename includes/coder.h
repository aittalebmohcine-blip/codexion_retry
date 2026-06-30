/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:59:23 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/22 11:10:17 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_H
# define CODER_H

# include "heap.h"
# include <pthread.h>

typedef enum e_coder_state
{
	STATE_IDLE,
	STATE_COMPILING,
	STATE_DEBUGGING,
	STATE_REFACTORING,
	STATE_BURNED_OUT,
	STATE_DONE
}	t_coder_state;

typedef struct s_dongle		t_dongle;
typedef struct s_simulation	t_simulation;

typedef struct s_coder
{
	pthread_t		thread;

	int				id;
	t_simulation	*sim;

	t_coder_state	state;
	long long		last_compile_time_ms;
	int				compiles_done;

	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	int				has_dongles;

	t_request		request;
	int				requested;
}	t_coder;

void	init_coders(t_simulation *sim);

int		compile(t_coder *coder);
void	debug(t_coder *coder);
void	refactor(t_coder *coder);

int		coder_is_done(t_coder *coder);
void	mark_coder_done(t_coder *coder);

void	set_state(t_coder *coder, t_coder_state state);
void	log_action(t_coder *coder, char *msg);
void	wait_for_start(t_simulation *sim);

void	*coder_routine(void *arg);

#endif
