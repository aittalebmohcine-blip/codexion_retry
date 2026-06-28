/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 11:00:22 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/22 11:18:02 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_H
# define PARSE_CONFIG_H

# include <sys/time.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

typedef enum e_scheduler_type
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler_type;

typedef struct s_config
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	t_scheduler_type	scheduler_type;
}	t_config;

int				parse_arguments(int argc, char **argv, t_config *config);
long long		get_time_ms(void);

//parse_config_helpers.c

/* CONSTRAINTS: Helper to print errors to stderr */
void			print_error(const char *msg);
int				is_valid_number(const char *str);

#endif
