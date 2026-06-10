#include "../includes/codexion.h"

long long	get_sim_time(t_simulation *sim)
{
	return (get_time_ms() - sim->start_time_ms);
}

static int	init_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

static void	init_coders(t_simulation *sim)
{
	int	i;
	int	n;

	n = sim->config.number_of_coders;
	i = 0;
	while (i < n)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % n];
		sim->coders[i].has_dongles = 0;
    sim->coders[i].state = STATE_IDLE;
		i++;
	}
}

static int	alloc_simulation(t_simulation *sim)
{
	int	n;

	n = sim->config.number_of_coders;
	sim->coders = malloc(sizeof(t_coder) * n);
	if (!sim->coders)
		return (0);
	sim->dongles = malloc(sizeof(t_dongle) * n);
	if (!sim->dongles)
	{
		free(sim->coders);
		return (0);
	}
	return (1);
}

int	init_simulation(t_simulation *sim, t_config *config)
{
	pthread_mutex_init(&sim->stop_mutex, NULL);
	sim->should_stop = 0;
	pthread_mutex_init(&sim->log_mutex, NULL);
	if (!sim || !config)
		return (0);
	sim->config = *config;
	sim->coders = NULL;
	sim->dongles = NULL;
	if (!alloc_simulation(sim))
		return (0);
	if (!init_dongles(sim))
	{
		free(sim->coders);
		free(sim->dongles);
		return (0);
	}
	init_coders(sim);
	return (1);
}

int	start_simulation(t_simulation *sim)
{
	int	i;

	sim->start_time_ms = get_time_ms();
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_create(
				&sim->coders[i].thread,
				NULL,
				coder_routine,
				&sim->coders[i]))
			return (0);
		i++;
	}
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim))
		return (0);
	return (1);
}

void	wait_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
}

int	simulation_stopped(t_simulation *sim)
{
	int	value;

	pthread_mutex_lock(&sim->stop_mutex);
	value = sim->should_stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (value);
}

void	stop_simulation(t_simulation *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->should_stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}
