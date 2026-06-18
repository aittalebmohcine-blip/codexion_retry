#include "../includes/heap.h"
#include "../includes/coder.h"

static void	swap_requests(t_request **a, t_request **b);
static void	heapify_up(t_heap *heap, int index);
static int	smallest_child(t_heap *heap, int index);
static void	heapify_down(t_heap *heap, int index);

void	heap_remove_top(t_heap *heap)
{
	if (heap->size == 0)
		return ;
	heap->size--;
	heap->data[0] = heap->data[heap->size];
	heapify_down(heap, 0);
}

void	heap_insert(t_heap *heap, t_request *request)
{
	if (heap->size >= MAX_CODERS)
		return ;
	heap->data[heap->size] = request;
	heapify_up(heap, heap->size);
	heap->size++;
}

//smaller priority wins
//equal priority → smaller coder id wins.
int	request_before(t_request *a, t_request *b)
{
	if (a->priority < b->priority)
		return (1);
	if (a->priority > b->priority)
		return (0);
	return (a->coder->id < b->coder->id);
}

t_request	*heap_top(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (heap->data[0]);
}

static void	swap_requests(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	heapify_up(t_heap *heap, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_before(heap->data[index],
				heap->data[parent]))
			break ;
		swap_requests(&heap->data[index],
			&heap->data[parent]);
		index = parent;
	}
}

static int	smallest_child(t_heap *heap, int index)
{
	int	left;
	int	right;

	left = index * 2 + 1;
	right = index * 2 + 2;
	if (right >= heap->size)
		return (left);
	if (request_before(heap->data[left],
			heap->data[right]))
		return (left);
	return (right);
}

static void	heapify_down(t_heap *heap, int index)
{
	int	child;

	while (index * 2 + 1 < heap->size)
	{
		child = smallest_child(heap, index);
		if (!request_before(heap->data[child],
				heap->data[index]))
			break ;
		swap_requests(&heap->data[child],
			&heap->data[index]);
		index = child;
	}
}
