#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "graph.h"

linked_list_t *ll_create(unsigned int data_size)
{
	linked_list_t *ll;
	
	ll = malloc(sizeof(linked_list_t));
	
	ll->head = NULL;
	ll->data_size = data_size;
	ll->size = 0;
	
	return ll;
}

ll_node_t* get_node(linked_list_t *list, void *value, int (*compare)(void *, void *))
{
	if (!list) {
		return NULL;
	}
	
	unsigned int len = list->size;
	unsigned int i;
	ll_node_t* node = list->head;
	for (i = 0; i < len; ++i) {
		if (!compare(node->data, value))
			return node;
        node = node->next;
	}
	
	return NULL;
}

unsigned int ll_get_pos(linked_list_t *list, void *value, int (*compare)(void *, void *)) {
    if (!list)
        return 0;

    unsigned int len = list->size;
    ll_node_t *node = list->head;
    for (unsigned int i = 0; i < len; i++) {
        if (!compare(node->data, value))
            return i;
        node = node->next;
    }

    return 0;
}

ll_node_t *ll_create_node(linked_list_t *list, unsigned int n) {
    ll_node_t *prev, *curr;
    ll_node_t *new_node;

    if (!list)
        return NULL;

    if (n > list->size)
        n = list->size;
    
    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(*new_node));
    new_node->data = malloc(list->data_size);
    new_node->next = curr;
    if (prev == NULL) {
		list->head = new_node;
	} else {
		prev->next = new_node;
	}
	
	list->size++;
    return new_node;
}

void
ll_add_nth_node(linked_list_t *list, unsigned int n, const void* new_data)
{
	ll_node_t *prev, *curr;
	ll_node_t* new_node;
	
	if (!list) {
		return;
	}
	
	/* n >= list->size inseamna adaugarea unui nou nod la finalul listei. */
	if (n > list->size) {
		n = list->size;
	}
	
	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}
	
	new_node = malloc(sizeof(*new_node));
	new_node->data = malloc(list->data_size);
	memcpy(new_node->data, new_data, list->data_size);
	
	new_node->next = curr;
	if (prev == NULL) {
		/* Adica n == 0. */
		list->head = new_node;
	} else {
		prev->next = new_node;
	}
	
	list->size++;
}

ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
	ll_node_t *prev, *curr;
	
	if (!list || !list->head) {
		return NULL;
	}
	
	/* n >= list->size - 1 inseamna eliminarea nodului de la finalul listei. */
	if (n > list->size - 1) {
		n = list->size - 1;
	}
	
	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}
	
	if (prev == NULL) {
		/* Adica n == 0. */
		list->head = curr->next;
	} else {
		prev->next = curr->next;
	}
	
	list->size--;
	
	return curr;
}

unsigned int
ll_get_size(linked_list_t* list)
{
	if (!list) {
		return -1;
	}
	
	return list->size;
}

void
ll_free(linked_list_t** pp_list, void (*free_func)(void *))
{
	ll_node_t* currNode;
	
	if (!pp_list || !*pp_list) {
		return;
	}
	
	while (ll_get_size(*pp_list) > 0) {
		currNode = ll_remove_nth_node(*pp_list, 0);
        if (currNode->data)
		    free_func(currNode->data);
		currNode->data = NULL;
		free(currNode);
		currNode = NULL;
	}
	
	free(*pp_list);
	*pp_list = NULL;
}

void
ll_print_int(linked_list_t* list)
{
	ll_node_t* curr;
	
	if (!list) {
		return;
	}
	
	curr = list->head;
	while (curr != NULL) {
		printf("%d ", *((int*)curr->data));
		curr = curr->next;
	}
	
	printf("\n");
}

list_graph_t*
lg_create(int nodes)
{
	/* TODO */
	list_graph_t *aux = malloc(sizeof(list_graph_t));
	aux->nodes = nodes;
	aux->neighbours = malloc(aux->nodes * sizeof(linked_list_t *));
	for (int i = 0 ; i < aux->nodes ; i++)
		aux->neighbours[i] = ll_create(sizeof(int));
	return aux;
}

void
lg_add_edge(list_graph_t* graph, int src, int dest)
{
	/* TODO */
	ll_add_nth_node(graph->neighbours[src], graph->neighbours[src]->size, &dest);
	ll_add_nth_node(graph->neighbours[dest], graph->neighbours[dest]->size, &src);
}

int
lg_has_edge(list_graph_t* graph, int src, int dest)
{
	/* TODO */
	ll_node_t *curr;
	curr = graph->neighbours[src]->head;
	while (curr) {
		if (*((int *)curr->data) == dest)
			return 1;
		curr = curr->next;
	}
	return 0;
}

void
lg_remove_edge(list_graph_t* graph, int src, int dest)
{
	/* TODO */
	ll_node_t *curr;
	curr = graph->neighbours[src]->head;
	int i = 0;
	ll_node_t *deleted_node;
	while (curr) {
		if (*((int *)curr->data) == dest) {
			deleted_node = ll_remove_nth_node(graph->neighbours[src], i);
			free(deleted_node->data);
			free(deleted_node);
			if (graph->neighbours[src]->size == 0)
				graph->neighbours[src]->head = NULL;
			break;
		}
		i++;
		curr = curr->next;
	}
	curr = graph->neighbours[dest]->head;
	i = 0;
	while (curr) {
		if (*((int *)curr->data) == src) {
			deleted_node = ll_remove_nth_node(graph->neighbours[dest], i);
			free(deleted_node->data);
			free(deleted_node);
			if (graph->neighbours[dest]->size == 0)
				graph->neighbours[dest]->head = NULL;
			return;
		}
		i++;
		curr = curr->next;
	}
}

void
lg_free(list_graph_t* graph)
{
	/* TODO */
	for (int i = 0 ; i < graph->nodes ; i++)
		ll_free(&graph->neighbours[i], free);
	free(graph->neighbours);
	free(graph);
}

void
lg_print_graph(list_graph_t* graph)
{
	/* TODO */
	for (int i = 0 ; i < graph->nodes ; i++) {
		printf("%d: ", i);
		ll_node_t *curr;
		curr = graph->neighbours[i]->head;
		while (curr) {
			printf("%d ", *((int*)curr->data));
			curr = curr->next;
		}
		printf("\n");
	}
}

queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t *q = calloc(1, sizeof(*q));
	DIE(!q, "calloc queue failed");

	q->data_size = data_size;
	q->max_size = max_size;

	q->buff = malloc(max_size * sizeof(*q->buff));
	DIE(!q->buff, "malloc buffer failed");

	return q;
}

unsigned int
q_get_size(queue_t *q)
{
	return !q ? 0 : q->size;
}

unsigned int
q_is_empty(queue_t *q)
{
	return !q ? 1 : !q->size;
}

void *
q_front(queue_t *q)
{
	if (!q || !q->size)
		return NULL;

	return q->buff[q->read_idx];
}

int q_dequeue(queue_t *q)
{
	if (!q || !q->size)
		return 0;

	free(q->buff[q->read_idx]);

	q->read_idx = (q->read_idx + 1) % q->max_size;
	--q->size;
	return 1;
}

int q_enqueue(queue_t *q, void *new_data)
{
	void *data;
	if (!q || q->size == q->max_size)
		return 0;

	data = malloc(q->data_size);
	DIE(!data, "malloc data failed");
	memcpy(data, new_data, q->data_size);

	q->buff[q->write_idx] = data;
	q->write_idx = (q->write_idx + 1) % q->max_size;
	++q->size;

	return 1;
}

void q_clear(queue_t *q)
{
	unsigned int i;
	if (!q || !q->size)
		return;

	for (i = q->read_idx; i != q->write_idx; i = (i + 1) % q->max_size)
		free(q->buff[i]);

	q->read_idx = 0;
	q->write_idx = 0;
	q->size = 0;
}

void q_free(queue_t *q)
{
	if (!q)
		return;

	q_clear(q);
	free(q->buff);
	free(q);
}
