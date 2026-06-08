#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef enum e_coder_state
{
	STATE_COMPILING,
	STATE_DEBUGGING,
	STATE_REFACTORING
}	t_coder_state;

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
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
	t_coder_state	state;

	struct s_simulation	*sim;

	long long	last_compile_time_ms;
}	t_coder;

typedef struct s_simulation
{
	t_config	config;
	t_coder		*coders;
	t_dongle	*dongles;

	pthread_mutex_t	log_mutex;
	long long	start_time_ms;
	pthread_t	monitor_thread;

	int	should_stop;
	pthread_mutex_t	stop_mutex;
}	t_simulation;

void	*monitor_routine(void *arg);

///
int	parse_arguments(int argc, char **argv, t_config *config);
long long	get_time_ms(void);

///
void	*coder_routine(void *arg);
void	log_action(t_coder *coder, char *msg);

int	take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);

void	compile(t_coder *coder);
void	debug(t_coder *coder);
void	refactor(t_coder *coder);
///

int		init_simulation(t_simulation *sim, t_config *config);
int		start_simulation(t_simulation *sim);
void	wait_simulation(t_simulation *sim);
void	destroy_simulation(t_simulation *sim);

long long	get_sim_time(t_simulation *sim);

int	simulation_stopped(t_simulation *sim);
void	stop_simulation(t_simulation *sim);
//
void	smart_sleep(t_simulation *sim, long long duration_ms);

#endif
