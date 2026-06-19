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

void	heap_remove_top(t_heap *heap);
void	heap_insert(t_heap *heap, t_request *request);
int	request_before(t_request *a, t_request *b);
t_request	*heap_top(t_heap *heap);
#endif
