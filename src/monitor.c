#include "../includes/codexion.h"

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				i;

	sim = (t_simulation *)arg;

	while (!simulation_stopped(sim))
	{
		i = 0;
		while (i < sim->config.number_of_coders)
		{
			if (get_sim_time(sim)
				- sim->coders[i].last_compile_time_ms
				> sim->config.time_to_burnout)
			{
        stop_simulation(sim);
				log_action(&sim->coders[i], "burned out");
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
