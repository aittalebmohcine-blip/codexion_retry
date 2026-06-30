/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:59:30 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/24 09:40:31 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_H
# define DONGLE_H

# include <unistd.h>
# include <pthread.h>
# include "heap.h"

typedef struct s_coder		t_coder;
typedef struct s_simulation	t_simulation;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				available;
	long long		next_available_time_ms;
	t_heap			waiters;
}	t_dongle;

//init_dongles.c
int		init_dongles(t_simulation *sim);

//dongle.c
int		take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);

//dongle_utils.c
void	lock_dongles(t_coder *coder);
void	unlock_dongles(t_coder *coder);
int		can_take(t_coder *coder);
void	add_request(t_coder *coder);
#endif
