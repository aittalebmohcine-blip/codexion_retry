/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 14:02:26 by mait-tal          #+#    #+#             */
/*   Updated: 2026/07/01 19:12:58 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse_config.h"

void	print_error(const char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
}

int	is_valid_number(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		str++;
		if (*str == '\0')
			return (0);
		while (*str == '0')
			str++;
		if (*str != '\0')
			return (0);
		return (1);
	}
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}
