#include "../includes/simulation.h"
#include "../includes/coder.h"
#include "../includes/dongle.h"
#include "../includes/monitor.h"
#include <pthread.h>

static int	alloc_simulation(t_simulation *sim);
static void	init_simulation_state(t_simulation *sim, t_config *config);
static int	init_simulation_mutexes(t_simulation *sim);
static void	destroy_simulation_mutexes(t_simulation *sim);
static int	alloc_and_init_resources(t_simulation *sim);

int	init_simulation(t_simulation *sim, t_config *config)
{
	if (!sim || !config)
		return (0);
	
	init_simulation_state(sim, config);
	
	if (!init_simulation_mutexes(sim))
		return (0);
	
	/* 1. Allocate resources FIRST so scheduler can depend on them */
	if (!alloc_and_init_resources(sim))
	{
		destroy_simulation_mutexes(sim);
		return (0);
	}
	
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

void	destroy_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	free(sim->coders);
	free(sim->dongles);
	destroy_simulation_mutexes(sim);
}

long long	get_sim_time(t_simulation *sim)
{
	return (get_time_ms() - sim->start_time_ms);
}

void	smart_sleep(t_simulation *sim, long long duration_ms)
{
	long long	start;

	start = get_time_ms();
	while (!simulation_stopped(sim))
	{
		if (get_time_ms() - start >= duration_ms)
			break ;
		usleep(500);
	}
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

static void	init_simulation_state(t_simulation *sim, t_config *config)
{
	sim->config = *config;
	sim->coders = NULL;
	sim->dongles = NULL;
	sim->should_stop = 0;
	sim->done_coders = 0;
}

static int	init_simulation_mutexes(t_simulation *sim)
{
	if (pthread_mutex_init(&sim->sim_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->sim_mutex);
		return (0);
	}
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->sim_mutex);
		pthread_mutex_destroy(&sim->stop_mutex);
		return (0);
	}
	if (pthread_mutex_init(&sim->wait_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->sim_mutex);
		pthread_mutex_destroy(&sim->stop_mutex);
		pthread_mutex_destroy(&sim->log_mutex);
		return (0);
	}
	if (pthread_cond_init(&sim->wait_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->sim_mutex);
		pthread_mutex_destroy(&sim->stop_mutex);
		pthread_mutex_destroy(&sim->log_mutex);
		pthread_mutex_destroy(&sim->wait_mutex);
		return (0);
	}
	return (1);
}

static void	destroy_simulation_mutexes(t_simulation *sim)
{
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->sim_mutex);
	pthread_mutex_destroy(&sim->wait_mutex);
  pthread_cond_destroy(&sim->wait_cond);
}

static int	alloc_and_init_resources(t_simulation *sim)
{
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
