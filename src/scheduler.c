#include "../includes/scheduler.h"
#include "../includes/coder.h"
#include "../includes/simulation.h"
#include "../includes/dongle.h"

int	init_scheduler(t_simulation *sim)
{
	t_scheduler	*scheduler;

	scheduler = &sim->scheduler;
	if (pthread_mutex_init(&scheduler->mutex, NULL))
		return (0);
	scheduler->size = 0;
	return (1);
}

void	destroy_scheduler(t_simulation *sim)
{
	pthread_mutex_destroy(&sim->scheduler.mutex);
}

int	scheduler_take_dongles(t_coder *coder)
{
	t_scheduler	*scheduler;

	scheduler = &coder->sim->scheduler;
	pthread_mutex_lock(&scheduler->mutex);
	if (!coder->left_dongle->is_available
		|| !coder->right_dongle->is_available)
	{
		pthread_mutex_unlock(&scheduler->mutex);
		return (0);
	}
	coder->left_dongle->is_available = 0;
	coder->right_dongle->is_available = 0;
	coder->has_dongles = 1;
	pthread_mutex_unlock(&scheduler->mutex);
	log_action(coder, "has taken a dongle");
	log_action(coder, "has taken a dongle");
	return (1);
}

void	scheduler_release_dongles(t_coder *coder)
{
	t_scheduler	*scheduler;

	if (!coder->has_dongles)
		return ;
	scheduler = &coder->sim->scheduler;
	pthread_mutex_lock(&scheduler->mutex);
	coder->left_dongle->is_available = 1;
	coder->right_dongle->is_available = 1;
	coder->has_dongles = 0;
	pthread_mutex_unlock(&scheduler->mutex);
}
