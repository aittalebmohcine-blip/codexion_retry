#include <pthread.h>
#include <unistd.h>
#include "../includes/dongle.h"
#include "../includes/coder.h"
#include "../includes/simulation.h"

static int	can_take(t_coder *coder);
static void	lock_dongles(t_coder *coder);
static void	unlock_dongles(t_coder *coder);
static void	add_request(t_coder *coder);

int	init_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (0);
		sim->dongles[i].waiters.size = 0;
		sim->dongles[i].available = 1;
		i++;
	}
	return (1);
}

int	take_dongles(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
		return (0);

	if (coder->id % 2 == 0)
		smart_sleep(coder->sim, (coder->sim->config.time_to_compile + coder->sim->config.dongle_cooldown) / 4);

	//request_dongles();
	add_request(coder);

	while (!can_take(coder) && !simulation_stopped(coder->sim))
		//use smart sleep for sim stop detection
		//if sim stoped return
		usleep(1000);
	if (simulation_stopped(coder->sim))
		return (0);

	lock_dongles(coder);
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
	if (!coder->has_dongles)
		return ;
	lock_dongles(coder);
	heap_remove_top(&coder->left_dongle->waiters);
	heap_remove_top(&coder->right_dongle->waiters);
	coder->requested = 0;
	coder->left_dongle->available = 1;
	coder->right_dongle->available = 1;
	coder->has_dongles = 0;
	unlock_dongles(coder);
}


static int	can_take(t_coder *coder)
{
	lock_dongles(coder);
	if (heap_top(&coder->left_dongle->waiters) != &coder->request)
	{
		unlock_dongles(coder);
		return (0);
	}
	if (heap_top(&coder->right_dongle->waiters) != &coder->request)
	{
		unlock_dongles(coder);
		return (0);
	}
	if (!coder->left_dongle->available || !coder->right_dongle->available)
	{
		unlock_dongles(coder);
		return (0);
	}
	unlock_dongles(coder);
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
	lock_dongles(coder);
	if (coder->sim->config.scheduler_type == SCHEDULER_FIFO)
		coder->request.priority = get_sim_time(coder->sim);
	else
		coder->request.priority = coder->last_compile_time_ms
			+ coder->sim->config.time_to_burnout;
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
