#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>

typedef enum e_scheduler
{
	SCHED_FIFO,
	SCHED_EDF
}	t_scheduler;

typedef struct s_config
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct s_coder
{
	int					id;
	pthread_t			thread;

	t_dongle	*left_dongle;
	t_dongle	*right_dongle;

	struct s_simulation	*sim;
}	t_coder;

typedef struct s_simulation
{
	t_config		config;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_simulation;

int	parse_arguments(int argc, char **argv, t_config *config);

int		init_simulation(t_simulation *sim, t_config *config);
int		start_simulation(t_simulation *sim);
void	wait_simulation(t_simulation *sim);
void	destroy_simulation(t_simulation *sim);

#endif
