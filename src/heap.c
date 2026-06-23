/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 11:01:01 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/23 11:03:22 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"
#include "coder.h"

void	heap_remove_top(t_heap *heap)
{
	if (heap->size == 0)
		return ;
	heap->size--;
	heap->data[0] = heap->data[heap->size];
	heapify_down(heap, 0);
}

void	heap_remove_request(t_heap *heap, t_request *request)
{
	int	i;

	if (heap->size == 0)
		return ;
	i = 0;
	while (i < heap->size && heap->data[i] != request)
		i++;
	if (i == heap->size)
		return ;
	heap->size--;
	if (i == heap->size)
		return ;
	heap->data[i] = heap->data[heap->size];
	if (i > 0 && request_before(heap->data[i], heap->data[(i - 1) / 2]))
		heapify_up(heap, i);
	else
		heapify_down(heap, i);
}

void	heap_insert(t_heap *heap, t_request *request)
{
	if (heap->size >= MAX_CODERS)
		return ;
	heap->data[heap->size] = request;
	heapify_up(heap, heap->size);
	heap->size++;
}

/* smaller priority wins.
** equal priority -> smaller coder id wins.
*/
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
