#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdbool.h>

/* ************************************************************************** */
/*                                 ENUMS                                      */
/* ************************************************************************** */

typedef enum e_scheduler_type
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler_type;

typedef enum e_coder_state
{
	STATE_THINKING,
	STATE_WAITING,
	STATE_COMPILING,
	STATE_DEBUGGING,
	STATE_REFACTORING,
	STATE_BURNED_OUT,
	STATE_FINISHED
}	t_coder_state;

/* ************************************************************************** */
/*                              DATA STRUCTURES                               */
/* ************************************************************************** */

/* Node for the custom Min-Heap (Priority Queue) */
typedef struct s_heap_node
{
	int			coder_id;
	long long	request_time_ms;
	long long	deadline_ms;
}	t_heap_node;

/* Custom Min-Heap for dongle waiting queues */
typedef struct s_scheduler_heap
{
	t_heap_node	*nodes;
	int			size;
	int			capacity;
}	t_scheduler_heap;

/* Represents a single coder thread */
typedef struct s_coder
{
	int				id;
	t_coder_state	state;
	int				compiles_completed;
	long long		last_compile_start_ms;
	long long		deadline_ms;
	pthread_t		thread_id;
}	t_coder;

/* Represents a single USB dongle resource */
typedef struct s_dongle
{
	int					id;
	bool				is_available;
	long long			cooldown_until_ms;
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	t_scheduler_heap	waiting_queue;
}	t_dongle;

/* Central simulation context (NO GLOBAL VARIABLES) */
typedef struct s_simulation
{
	/* Configuration (from command-line arguments) */
	int				num_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				compiles_required;
	long long		dongle_cooldown;
	t_scheduler_type	scheduler;

	/* Simulation State */
	bool			is_running;
	int				active_coders; /* Tracks coders that haven't finished/burned out */

	/* Entities */
	t_coder			*coders;
	t_dongle		*dongles;

	/* Synchronization Primitives */
	pthread_mutex_t	log_mutex;  /* Protects all console output (serialization) */
	pthread_mutex_t	sim_mutex;  /* Protects sim state (is_running, active_coders) */
	pthread_cond_t	sim_cond;   /* Allows main thread to wait for monitor termination */
}	t_simulation;

/* ************************************************************************** */
/*                            FUNCTION PROTOTYPES                             */
/* ************************************************************************** */

/* --- main.c --- */
int			main(int argc, char **argv);

/* --- simulation.c --- */
int			simulation_init(t_simulation *sim, char **argv);
void		simulation_destroy(t_simulation *sim);

/* --- coder.c --- */
void		*coder_routine(void *arg);

/* --- dongle.c --- */
int			acquire_dongles(t_simulation *sim, int coder_id);
void		release_dongles(t_simulation *sim, int coder_id);

/* --- scheduler.c --- */
int			heap_init(t_scheduler_heap *heap, int capacity);
void		heap_destroy(t_scheduler_heap *heap);
int			heap_push(t_simulation *sim, t_scheduler_heap *heap, int coder_id, long long request_time, long long deadline);
int			heap_pop(t_simulation *sim, t_scheduler_heap *heap, int *out_coder_id);
int			heap_is_empty(t_scheduler_heap *heap);

/* --- monitor.c --- */
void		*monitor_routine(void *arg);

/* --- utils.c --- */
long long	get_current_time_ms(void);
void		safe_log(t_simulation *sim, long long timestamp, int coder_id, const char *message);
bool		is_valid_number(const char *str);
t_scheduler_type parse_scheduler(const char *str);
void		*ft_calloc(size_t count, size_t size);

int	parse_arguments(t_simulation *sim, int argc, char **argv);

#endif /* CODEXION_H */
