/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:05:12 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/23 11:05:13 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

void	swap_requests(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heapify_up(t_heap *heap, int index)
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

int	smallest_child(t_heap *heap, int index)
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

void	heapify_down(t_heap *heap, int index)
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
