#include <pthread.h>
#include <unistd.h>
#include "dongle.h"
#include "coder.h"
#include "simulation.h"

static int	can_take(t_coder *coder);
static void	lock_dongles(t_coder *coder);
static void	unlock_dongles(t_coder *coder);
static void	add_request(t_coder *coder);

int	init_dongles(t_simulation *sim)
{
	int		i;
	int		j;

	if (!sim || !sim->dongles || sim->config.number_of_coders <= 0)
		return (0);

	/* clear initialized count first */
	sim->initialized_dongles = 0;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
		{
			j = 0;
			/* destroy any mutexes successfully initialized so far */
			while (j < i)
			{
				pthread_mutex_destroy(&sim->dongles[j].mutex);
				j++;
			}
			sim->initialized_dongles = 0;
			return (0);
		}
		sim->dongles[i].waiters.size = 0;
		sim->dongles[i].available = 1;
		sim->dongles[i].next_available_time_ms = 0;
		i++;
		sim->initialized_dongles++;
	}
	return (1);
}

int	take_dongles(t_coder *coder)
{
	long long	sleep_delay;

	if (coder->left_dongle == coder->right_dongle)
		return (0);

	if (coder->id % 2 == 0)
	{
		sleep_delay = coder->sim->config.time_to_compile
			+ coder->sim->config.dongle_cooldown;
		smart_sleep(coder->sim, sleep_delay / 4);
	}

	add_request(coder);

	while (!simulation_stopped(coder->sim))
	{
		lock_dongles(coder);
		if (can_take(coder))
			break ;
		unlock_dongles(coder);
		usleep(1000);
	}
	if (simulation_stopped(coder->sim))
		return (0);

	coder->left_dongle->available = 0;
	coder->right_dongle->available = 0;
	coder->has_dongles = 1;
	log_action(coder, "has taken a dongle");
	log_action(coder, "has taken a dongle");
	unlock_dongles(coder);
	return (1);
}

void	release_dongles(t_coder *coder)
{
	long long	now;

	if (!coder->has_dongles)
		return ;
	now = get_sim_time(coder->sim);
	lock_dongles(coder);
	heap_remove_request(&coder->left_dongle->waiters,
				&coder->request);
	heap_remove_request(&coder->right_dongle->waiters,
				&coder->request);
	coder->requested = 0;
	coder->left_dongle->available = 1;
	coder->left_dongle->next_available_time_ms = now
		+ coder->sim->config.dongle_cooldown;
	coder->right_dongle->available = 1;
	coder->right_dongle->next_available_time_ms = now
		+ coder->sim->config.dongle_cooldown;
	coder->has_dongles = 0;
	unlock_dongles(coder);
}


static int	can_take(t_coder *coder)
{
	long long	now;

	now = get_sim_time(coder->sim);
	if (heap_top(&coder->left_dongle->waiters) != &coder->request)
		return (0);
	if (heap_top(&coder->right_dongle->waiters) != &coder->request)
		return (0);
	if (!coder->left_dongle->available
		|| now < coder->left_dongle->next_available_time_ms
		|| !coder->right_dongle->available
		|| now < coder->right_dongle->next_available_time_ms)
		return (0);
	return (1);
}

static void	lock_dongles(t_coder *coder)
{
	if (coder->left_dongle < coder->right_dongle)
	{
		pthread_mutex_lock(&coder->left_dongle->mutex);
		pthread_mutex_lock(&coder->right_dongle->mutex);
	}
	else
	{
		pthread_mutex_lock(&coder->right_dongle->mutex);
		pthread_mutex_lock(&coder->left_dongle->mutex);
	}
}

static void	unlock_dongles(t_coder *coder)
{
	if (coder->left_dongle < coder->right_dongle)
	{
		pthread_mutex_unlock(&coder->right_dongle->mutex);
		pthread_mutex_unlock(&coder->left_dongle->mutex);
	}
	else
	{
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		pthread_mutex_unlock(&coder->right_dongle->mutex);
	}
}

static void	add_request(t_coder *coder)
{
	long long	deadline;

	lock_dongles(coder);
	if (coder->sim->config.scheduler_type == SCHEDULER_FIFO)
		coder->request.priority = get_sim_time(coder->sim);
	else
	{
		pthread_mutex_lock(&coder->sim->sim_mutex);
		deadline = coder->last_compile_time_ms;
		pthread_mutex_unlock(&coder->sim->sim_mutex);
		coder->request.priority = deadline
			+ coder->sim->config.time_to_burnout;
	}
	if (!coder->requested)
	{
		heap_insert(&coder->left_dongle->waiters,
			&coder->request);
		heap_insert(&coder->right_dongle->waiters,
			&coder->request);
		coder->requested = 1;
	}
	unlock_dongles(coder);
}
