#include <pthread.h>
#include <unistd.h>
#include "../includes/dongle.h"
#include "../includes/coder.h"
#include "../includes/simulation.h"

static void	get_dongle_order(t_coder *coder,
	pthread_mutex_t **first,
	pthread_mutex_t **second);
//static int	lock_first_dongle(t_coder *coder, pthread_mutex_t *first);
static int	can_take(t_coder *coder);

int	init_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->dongles[i].available = 1;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	take_dongles(t_coder *coder)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	//coder should not aquire any dongle anless he can explecitly take both left and right
	//maybe check if coder allready has dongles beffore taking
	if (coder->left_dongle == coder->right_dongle)
		return (0);

	if (coder->id % 2 == 0)
		usleep(1000);
	get_dongle_order(coder, &first, &second);

	//request_dongles();

	while (!can_take(coder))
		usleep(1000);

	pthread_mutex_lock(first);
	pthread_mutex_lock(second);

	coder->left_dongle->available = 0;
	coder->right_dongle->available = 0;
	coder->has_dongles = 1;

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
	coder->left_dongle->available = 1;
	coder->right_dongle->available = 1;
	coder->has_dongles = 0;
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

//static int	lock_first_dongle(t_coder *coder, pthread_mutex_t *first)
//{
//	if (coder->left_dongle < coder->right_dongle && !coder->left_dongle->available)
//		return (0);
//	else if (coder->left_dongle > coder->right_dongle && !coder->right_dongle->available)
//		return (0);
//	pthread_mutex_lock(first);
//	//log_action(coder, "has taken a dongle");
//	if (simulation_stopped(coder->sim))
//	{
//		pthread_mutex_unlock(first);
//		return (0);
//	}
//	if (coder->left_dongle < coder->right_dongle)
//		coder->left_dongle->available = 0;
//	else
//		coder->right_dongle->available = 0;
//	return (1);
//}

static int	can_take(t_coder *coder)
{
	if (!coder->left_dongle->available || !coder->right_dongle->available)
		return (0);
	//the coder need to be in the top of the left and right dongles heap
	return (1);
}
