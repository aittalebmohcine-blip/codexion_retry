/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-tal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:59:35 by mait-tal          #+#    #+#             */
/*   Updated: 2026/06/24 09:38:10 by mait-tal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

# define MAX_CODERS 2

typedef struct s_coder	t_coder;

typedef struct s_request
{
	t_coder		*coder;
	long long	priority;
}	t_request;

typedef struct s_heap
{
	t_request	*data[MAX_CODERS];
	int			size;
}	t_heap;

void			heap_remove_top(t_heap *heap);
void			heap_remove_request(t_heap *heap, t_request *request);
void			heap_insert(t_heap *heap, t_request *request);
int				request_before(t_request *a, t_request *b);
t_request		*heap_top(t_heap *heap);

void			heapify_down(t_heap *heap, int index);
void			heapify_up(t_heap *heap, int index);
void			swap_requests(t_request **a, t_request **b);
int				smallest_child(t_heap *heap, int index);
#endif
