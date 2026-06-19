#include <pthread.h>
#include <unistd.h>
#include "../includes/dongle.h"
#include "../includes/coder.h"
#include "../includes/simulation.h"

static int	can_take(t_coder *coder);

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

	while (!can_take(coder))
		//use smart sleep for sim stop detection
		//if sim stoped return
		usleep(1000);

	pthread_mutex_lock(&coder->right_dongle->mutex);
	pthread_mutex_lock(&coder->left_dongle->mutex);

	coder->left_dongle->available = 0;
	coder->right_dongle->available = 0;
	coder->has_dongles = 1;
	log_action(coder, "has taken a dongle");
	log_action(coder, "has taken a dongle");

	pthread_mutex_unlock(&coder->right_dongle->mutex);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	return (1);
}

void	release_dongles(t_coder *coder)
{
	if (!coder->has_dongles)
		return ;
	pthread_mutex_lock(&coder->right_dongle->mutex);
	pthread_mutex_lock(&coder->left_dongle->mutex);
	coder->left_dongle->available = 1;
	coder->right_dongle->available = 1;
	coder->has_dongles = 0;
	pthread_mutex_unlock(&coder->right_dongle->mutex);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
}


static int	can_take(t_coder *coder)
{
	pthread_mutex_lock(&coder->right_dongle->mutex);
	pthread_mutex_lock(&coder->left_dongle->mutex);

	if (!coder->left_dongle->available || !coder->right_dongle->available)
	{
		pthread_mutex_unlock(&coder->right_dongle->mutex);
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		return (0);
	}
	pthread_mutex_unlock(&coder->right_dongle->mutex);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	return (1);
}
