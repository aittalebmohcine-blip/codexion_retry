/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 14:02:26 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/28 14:02:28 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse_config.h"

/* CONSTRAINTS: Helper to print errors to stderr */
void	print_error(const char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
}

/* CONSTRAINTS: Strict integer validation (no for loops) */
int	is_valid_number(const char *str)
{
	if (!str || *str == '\0' || *str == '-')
		return (0);
	if (*str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}
