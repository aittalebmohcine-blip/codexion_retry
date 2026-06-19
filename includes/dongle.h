#ifndef DONGLE_H
# define DONGLE_H

#include "heap.h"
#include <pthread.h>

typedef struct s_coder t_coder;
typedef struct s_simulation t_simulation;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int	available;
	t_heap	waiters;
}	t_dongle;

int	init_dongles(t_simulation *sim);
int	take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);

#endif
