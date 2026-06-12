#ifndef PARSE_CONFIG_H
# define PARSE_CONFIG_H

#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int	parse_arguments(int argc, char **argv, t_config *config);
long long	get_time_ms(void);

#endif
