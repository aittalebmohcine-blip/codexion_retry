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

static void	print_error(const char *msg);
static int	parse_int_field(const char *arg, int *out, int must_be_positive);

/* TRANSITIONS: Raw argv → validated config state (21 lines) */
int	parse_arguments(int argc, char **argv, t_config *config)
{
	if (argc != 9)
		return (print_error("Invalid number of arguments."), 0);
	if (!parse_int_field(argv[1], &config->number_of_coders, 1)
		|| !parse_int_field(argv[2], &config->time_to_burnout, 1)
		|| !parse_int_field(argv[3], &config->time_to_compile, 1))
		return (print_error("Invalid positive integer argument."), 0);
	if (!parse_int_field(argv[4], &config->time_to_debug, 0)
		|| !parse_int_field(argv[5], &config->time_to_refactor, 0)
		|| !parse_int_field(argv[6], &config->number_of_compiles_required, 1)
		|| !parse_int_field(argv[7], &config->dongle_cooldown, 0))
		return (print_error("Invalid integer argument."), 0);
	if (strcmp(argv[8], "fifo") == 0)
		config->scheduler_type = SCHEDULER_FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		config->scheduler_type = SCHEDULER_EDF;
	else
		return (print_error("Invalid scheduler. Use 'fifo' or 'edf'."), 0);
	return (1);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

/* CONSTRAINTS: Helper to print errors to stderr */
static void	print_error(const char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
}

/* CONSTRAINTS: Strict integer validation (no for loops) */
static int	is_valid_number(const char *str)
{
	if (!str || *str == '\0' || *str == '-')
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
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
