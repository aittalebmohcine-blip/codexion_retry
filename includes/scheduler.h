#ifndef SCHEDULER_H
# define SCHEDULER_H

#include <pthread.h>

typedef struct s_coder t_coder;
typedef struct s_simulation t_simulation;

typedef struct s_request
{
	t_coder		*coder;
	long long	timestamp;
}	t_request;

typedef struct s_scheduler
{
	pthread_mutex_t	mutex;

	t_request		queue[10];
	int				size;
}	t_scheduler;

int	init_scheduler(t_simulation *sim);
void	destroy_scheduler(t_simulation *sim);

int		scheduler_take_dongles(t_coder *coder);
void	scheduler_release_dongles(t_coder *coder);

#endif
