#include "../includes/codexion.h"
#include <pthread.h>

static void	set_state(t_coder *coder, t_coder_state state)
{
	coder->state = state;
	if (state == STATE_COMPILING)
		log_action(coder, "is compiling");
	else if (state == STATE_DEBUGGING)
		log_action(coder, "is debugging");
	else if (state == STATE_REFACTORING)
		log_action(coder, "is refactoring");
}

static void	get_dongle_order(t_coder *coder,
	pthread_mutex_t **first,
	pthread_mutex_t **second)
{
	if (coder->left_dongle < coder->right_dongle)
	{
		*first = &coder->left_dongle->mutex;
		*second = &coder->right_dongle->mutex;
	}
	else
	{
		*first = &coder->right_dongle->mutex;
		*second = &coder->left_dongle->mutex;
	}
}

static int	lock_first_dongle(t_coder *coder, pthread_mutex_t *first)
{
	pthread_mutex_lock(first);
	//log_action(coder, "has taken a dongle");
	if (simulation_stopped(coder->sim))
	{
		pthread_mutex_unlock(first);
		return (0);
	}
	return (1);
}

int	take_dongles(t_coder *coder)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (coder->left_dongle == coder->right_dongle)
		return (0);
	get_dongle_order(coder, &first, &second);
	if (!lock_first_dongle(coder, first))
		return (0);
	pthread_mutex_lock(second);
	//log_action(coder, "has taken a dongle");
	if (simulation_stopped(coder->sim))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return (0);
	}
	coder->has_dongles = 1;
	log_action(coder, "has taken a dongle");
	log_action(coder, "has taken a dongle");
	return (1);
}

void	release_dongles(t_coder *coder)
{
	if (!coder->has_dongles)
		return ;
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
	coder->has_dongles = 0;
}

void	compile(t_coder *coder)
{
	if (simulation_stopped(coder->sim))
	{
		release_dongles(coder);
		return ;
	}
	coder->last_compile_time_ms = get_sim_time(coder->sim);
	set_state(coder, STATE_COMPILING);
	smart_sleep(coder->sim,
		coder->sim->config.time_to_compile);
}

void	debug(t_coder *coder)
{
	if (simulation_stopped(coder->sim))
		return ;
	set_state(coder, STATE_DEBUGGING);
	smart_sleep(coder->sim,
		coder->sim->config.time_to_debug);
}

void	refactor(t_coder *coder)
{
	if (simulation_stopped(coder->sim))
		return ;
	set_state(coder, STATE_REFACTORING);
	smart_sleep(coder->sim,
		coder->sim->config.time_to_refactor);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!simulation_stopped(coder->sim))
	{
		if (!take_dongles(coder))
			break ;
		compile(coder);
		release_dongles(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}

void	log_action(t_coder *coder, char *msg)
{
	pthread_mutex_lock(&coder->sim->log_mutex);

	printf("%lld %d %s\n",
		get_sim_time(coder->sim),
		coder->id,
		msg);

	pthread_mutex_unlock(&coder->sim->log_mutex);
}
