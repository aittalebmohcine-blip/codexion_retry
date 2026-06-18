#ifndef HEAP_H
# define HEAP_H

#define MAX_CODERS 100

typedef struct s_request
{
	struct s_coder	*coder;
	long long		priority;
}	t_request;

typedef struct s_heap
{
	t_request	*data[MAX_CODERS];
	int			size;
}	t_heap;

#endif
