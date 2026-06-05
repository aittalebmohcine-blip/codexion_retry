#ifndef CODEXION_H
# define CODEXION_H

typedef enum e_coder_state
{
	IDLE,
	WORKING
}	t_coder_state;

typedef struct s_coder
{
	int				id;
	t_coder_state	state;
}	t_coder;

typedef struct s_resource
{
	int				owner_id;
}	t_resource;

typedef struct s_simulation
{
	int				running;
	t_coder		*coders;
	t_resource		resource;
}	t_simulation;

#endif /* CODEXION_H */
