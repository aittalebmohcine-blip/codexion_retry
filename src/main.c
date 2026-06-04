#include "../includes/codexion.h"

int	main(int argc, char **argv)
{
	t_simulation	sim;
  // 1. Parse and validate command-line arguments
  if (parse_arguments(&sim, argc, argv) != 0)
        return (1);
}
