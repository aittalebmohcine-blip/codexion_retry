/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 11:01:05 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/24 09:28:48 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/simulation.h"

int	main(int argc, char **argv)
{
	t_config		config;
	t_simulation	sim;

	memset(&config, 0, sizeof(t_config));
	if (!parse_arguments(argc, argv, &config))
		return (1);
	if (!init_simulation(&sim, &config))
		return (1);
	if (!start_simulation(&sim))
		return (1);
	wait_simulation(&sim);
	destroy_simulation(&sim);
	return (0);
}
