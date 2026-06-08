#include "../includes/codexion.h"

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

void	take_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(&coder->right_dongle->mutex);
		log_action(coder, "has taken a dongle");
		pthread_mutex_lock(&coder->left_dongle->mutex);
		log_action(coder, "has taken a dongle");
	}
	else
	{
		pthread_mutex_lock(&coder->left_dongle->mutex);
		log_action(coder, "has taken a dongle");
		pthread_mutex_lock(&coder->right_dongle->mutex);
		log_action(coder, "has taken a dongle");
	}
}

void	release_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
}

void	compile(t_coder *coder)
{
	coder->last_compile_time_ms = get_sim_time(coder->sim);
	set_state(coder, STATE_COMPILING);
	smart_sleep(coder->sim,
		coder->sim->config.time_to_compile);
}

void	debug(t_coder *coder)
{
	set_state(coder, STATE_DEBUGGING);
	smart_sleep(coder->sim,
		coder->sim->config.time_to_debug);
}

void	refactor(t_coder *coder)
{
	set_state(coder, STATE_REFACTORING);
	smart_sleep(coder->sim,
		coder->sim->config.time_to_refactor);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!coder->sim->should_stop)
	{
		take_dongles(coder);

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
