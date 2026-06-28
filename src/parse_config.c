/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 11:01:15 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/24 09:28:56 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse_config.h"

static int	parse_int_field(const char *arg, int *out, int must_be_positive);
static int	valid_argc(int argc);
static int	cheduler_type(char *s);

/* TRANSITIONS: Raw argv → validated config state (21 lines) */
int	parse_arguments(int argc, char **argv, t_config *config)
{
	if (!valid_argc(argc))
		return (0);
	if (!parse_int_field(argv[1], &config->number_of_coders, 1)
		|| !parse_int_field(argv[2], &config->time_to_burnout, 1)
		|| !parse_int_field(argv[3], &config->time_to_compile, 1))
	{
		print_error("Invalid positive integer argument.");
		return (0);
	}
	if (!parse_int_field(argv[4], &config->time_to_debug, 1)
		|| !parse_int_field(argv[5], &config->time_to_refactor, 1)
		|| !parse_int_field(argv[6], &config->number_of_compiles_required, 1)
		|| !parse_int_field(argv[7], &config->dongle_cooldown, 1))
	{
		print_error("Invalid integer argument.");
		return (0);
	}
	if (cheduler_type(argv[8]) == 0)
		return (0);
	else if (cheduler_type(argv[8]) == 1)
		config->scheduler_type = SCHEDULER_FIFO;
	else if (cheduler_type(argv[8]) == 2)
		config->scheduler_type = SCHEDULER_EDF;
	return (1);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

/* CONSTRAINTS: Validates and populates a single integer field */
static int	parse_int_field(const char *arg, int *out, int must_be_positive)
{
	int	val;

	if (!is_valid_number(arg))
		return (0);
	val = atoi(arg);
	if (must_be_positive && val <= 0)
		return (0);
	*out = val;
	return (1);
}

static int	valid_argc(int argc)
{
	if (argc != 9)
	{
		print_error("Invalid number of arguments.");
		return (0);
	}
	return (1);
}

static int	cheduler_type(char *s)
{
	if (strcmp(s, "fifo") == 0)
		return (1);
	else if (strcmp(s, "edf") == 0)
		return (2);
	else
	{
		print_error("Invalid scheduler. Use 'fifo' or 'edf'.");
		return (0);
	}
}
