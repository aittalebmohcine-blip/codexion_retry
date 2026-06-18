#include "../includes/coder.h"
#include "../includes/simulation.h"
#include "../includes/dongle.h"

void	init_coders(t_simulation *sim)
{
	int	i;
	int	n;

	n = sim->config.number_of_coders;
	i = 0;
	while (i < n)
	{
		sim->coders[i].id = i + 1;


		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % n];

		sim->coders[i].has_dongles = 0;

    sim->coders[i].state = STATE_IDLE;

		sim->coders[i].sim = sim;

    sim->coders[i].last_compile_time_ms = 0;

    sim->coders[i].compiles_done = 0;
		i++;
	}
}

int	compile(t_coder *coder)
{
	if (simulation_stopped(coder->sim))
		return (0);
	coder->last_compile_time_ms = get_sim_time(coder->sim);
	set_state(coder, STATE_COMPILING);
	smart_sleep(coder->sim,
		coder->sim->config.time_to_compile);
	if (simulation_stopped(coder->sim))
		return (0);
	coder->compiles_done++;
	if (coder_is_done(coder))
	{
		set_state(coder, STATE_DONE);
    mark_coder_done(coder);
		return (0);
	}
	return (1);
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

int	coder_is_done(t_coder *coder)
{
	if (coder->sim->config.number_of_compiles_required <= 0)
		return (0);
	return (coder->compiles_done
		>= coder->sim->config.number_of_compiles_required);
}

void	mark_coder_done(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->sim_mutex);
	coder->sim->done_coders++;
	pthread_mutex_unlock(&coder->sim->sim_mutex);
}

void	set_state(t_coder *coder, t_coder_state state)
{
	coder->state = state;
	if (state == STATE_COMPILING)
		log_action(coder, "is compiling");
	else if (state == STATE_DEBUGGING)
		log_action(coder, "is debugging");
	else if (state == STATE_REFACTORING)
		log_action(coder, "is refactoring");
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

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
  while (!simulation_stopped(coder->sim))
  {
  	if (!take_dongles(coder))
  		break ;
  	if (!compile(coder))
  	{
  		release_dongles(coder);
  		break ;
  	}
  	release_dongles(coder);
  	debug(coder);
  	refactor(coder);
  }
	return (NULL);
}
