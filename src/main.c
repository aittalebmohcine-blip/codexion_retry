#include "../includes/codexion.h"

/* ENTRY POINT */
int	main(int argc, char **argv)
{
	t_config	config;

	memset(&config, 0, sizeof(t_config));
	if (!parse_arguments(argc, argv, &config))
		return (1);
	return (0);
}
