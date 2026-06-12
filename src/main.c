#include "../includes/simulation.h"

/* ENTRY POINT */
int	main(int argc, char **argv)
{
	t_config	config;
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
