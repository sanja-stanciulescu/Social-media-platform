#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "friends.h"
#include "users.h"

void handle_input_friends(char *input, list_graph_t *graph)
{
	char *commands = strdup(input);

	if (!commands)
		return;

	if (!strncmp(commands, "add", 3)) {
		char name_1[MAX_NAME_LEN], name_2[MAX_NAME_LEN];

		sscanf(commands, "%*s %s %s\n", name_1, name_2);
		add_friends(graph, name_1, name_2);

	} else if (!strncmp(commands, "remove", 6)) {
		char name_1[MAX_NAME_LEN], name_2[MAX_NAME_LEN];

		sscanf(commands, "%*s %s %s\n", name_1, name_2);
		remove_friends(graph, name_1, name_2);
	} else if (!strncmp(commands, "suggestions", 11)) {
		char name_1[MAX_NAME_LEN];

		sscanf(commands, "%*s %s\n", name_1);
		friend_suggestions(graph, name_1);
	} else if (!strncmp(commands, "distance", 8)) {
		char name_1[MAX_NAME_LEN], name_2[MAX_NAME_LEN];

		sscanf(commands, "%*s %s %s\n", name_1, name_2);
		distance_friends(graph, name_1, name_2);
	} else if (!strncmp(commands, "common ", 7)) {
		char name_1[MAX_NAME_LEN], name_2[MAX_NAME_LEN];

		sscanf(commands, "%*s %s %s\n", name_1, name_2);
		common_friends(graph, name_1, name_2);
		
	} else if (!strncmp(commands, "friends ", 8)) {
		char name_1[MAX_NAME_LEN];

		sscanf(commands, "%*s %s\n", name_1);
		friends_number(graph, name_1);
	} else if (!strncmp(commands, "popular", 7)) {
		char name_1[MAX_NAME_LEN];

		sscanf(commands, "%*s %s\n", name_1);
		popular_friend(graph, name_1);
	}		

	free(commands);
}

void add_friends(list_graph_t *graph, char *name_1, char *name_2) {
	int id1 = (int)get_user_id(name_1);
	int id2 = (int)get_user_id(name_2);
	lg_add_edge(graph, id1, id2);
	printf("Added connection %s - %s\n", name_1, name_2);
}

void remove_friends(list_graph_t *graph, char *name_1, char *name_2) {
	int id1 = (int)get_user_id(name_1);
	int id2 = (int)get_user_id(name_2);
	lg_remove_edge(graph, id1, id2);
	printf("Removed connection %s - %s\n", name_1, name_2);
}

void distance_friends(list_graph_t *graph, char *name_1, char *name_2) {
	int src = (int)get_user_id(name_1);
	int dest = (int)get_user_id(name_2);

	queue_t *q;
	q = q_create(sizeof(int), graph->nodes + 1);
	int *visited = calloc((graph->nodes + 1), sizeof(int));
	int *nodeTop, n, *distance;

	distance = malloc(graph->nodes * sizeof(int));
	for (int i = 0; i < graph->nodes; i++) {
		distance[i] = INF;
	}

	nodeTop = &src;
	q_enqueue(q, nodeTop);
	visited[src] = 1;
	distance[src] = 0;

	while (q != NULL && q->size != 0) {
		nodeTop = (int *)q_front(q);
		n = *nodeTop;
		q_dequeue(q);

		ll_node_t *curr_node = graph->neighbours[n]->head;
		for (unsigned int i = 0; i < graph->neighbours[n]->size; i++) {
			if (visited[*((int *)curr_node->data)] == 0) {
				q_enqueue(q, curr_node->data);
				visited[*((int *)curr_node->data)]++;
				distance[*((int *)curr_node->data)] = distance[n] + 1;
			}
			curr_node = curr_node->next;
		}
	}

	if (distance[dest] == INF) {
		printf("There is no way to get from %s to %s\n", name_1, name_2);
	} else {
		printf("The distance between %s - %s is %d\n", name_1, name_2, distance[dest]);
	}
	free(distance);
	free(visited);
	q_free(q);
}

void selection_sort(int *arr, int n) {
	int i, j, min_idx;

    for (i = 0; i < n-1; i++)
    {
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] < arr[min_idx])
            min_idx = j;

        if(min_idx != i) {
			int temp = arr[min_idx];
			arr[min_idx] = arr[i];
			arr[i] = temp;
		}
    }
}

void friend_suggestions(list_graph_t *graph, char *name) {
	int src = (int)get_user_id(name);

	queue_t *q;
	q = q_create(sizeof(int), graph->nodes + 1);
	int *visited = calloc((graph->nodes + 1), sizeof(int));
	int *nodeTop, n, *distance;

	distance = malloc(graph->nodes * sizeof(int));
	for (int i = 0; i < graph->nodes; i++) {
		distance[i] = INF;
	}

	nodeTop = &src;
	q_enqueue(q, nodeTop);
	visited[src] = 1;
	distance[src] = 0;

	int i = 1 + graph->neighbours[src]->size;
	while (i != 0) {
		nodeTop = (int *)q_front(q);
		n = *nodeTop;
		q_dequeue(q);

		ll_node_t *curr_node = graph->neighbours[n]->head;
		for (unsigned int i = 0; i < graph->neighbours[n]->size; i++) {
			if (visited[*((int *)curr_node->data)] == 0) {
				q_enqueue(q, curr_node->data);
				visited[*((int *)curr_node->data)]++;
				distance[*((int *)curr_node->data)] = distance[n] + 1;
			}
			curr_node = curr_node->next;
		}
		i--;
	}
	unsigned int len = q->size;
	int *suggested_friends = malloc(len * sizeof(int));
	for (unsigned int i = 0; i < len; i++) {
		suggested_friends[i] = *(int *)q_front(q);
		q_dequeue(q);
	}
	selection_sort(suggested_friends, len);
    if (len == 0) {
        printf("There are no suggestions for %s\n", name);
    } else {
	    printf("Suggestions for %s:\n", name);
	    for (unsigned int i = 0; i < len; i++) {
	    	printf("%s\n", get_user_name(suggested_friends[i]));
	    }
    }

	free(suggested_friends);
	free(distance);
	free(visited);
	q_free(q);
}

void common_friends(list_graph_t *graph, char *name_1, char *name_2) {
	int src = (int)get_user_id(name_1);
	int dest = (int)get_user_id(name_2);

	int f_1, f_2;
	int len = MIN(graph->neighbours[src]->size, graph->neighbours[dest]->size);
	int *com_friends = malloc(len * sizeof(int));
	int ok = 0;

	ll_node_t *curr_node_1 = graph->neighbours[src]->head;
	for (unsigned int i = 0; i < graph->neighbours[src]->size; i++) {
		f_1 = *(int *)curr_node_1->data;
		ll_node_t *curr_node_2 = graph->neighbours[dest]->head;
		for (unsigned j = 0; j < graph->neighbours[dest]->size; j++) {
			f_2 = *(int *)curr_node_2->data;
			if (f_1 == f_2) {
				com_friends[ok] = f_1;
				ok++;
				break;
			}
			curr_node_2 = curr_node_2->next;
		}
		curr_node_1 = curr_node_1->next;
	}
	
	if ( ok == 0) {
		printf("No common friends for %s and %s\n", name_1, name_2);
	} else {
		printf("The common friends between %s and %s are:\n", name_1, name_2);
		selection_sort(com_friends, ok);
		for (int i = 0; i < ok; i++)
			printf("%s\n", get_user_name(com_friends[i])); 
	}

	free(com_friends);
}

void friends_number(list_graph_t *graph, char *name) {
	int id = get_user_id(name);
	printf("%s has %d friends\n", name, graph->neighbours[id]->size);
}

void popular_friend(list_graph_t *graph, char *name) {
	int id = get_user_id(name);
	int max = id, ok = 0;
	ll_node_t *curr_node = graph->neighbours[id]->head;
	for (unsigned int i = 0; i < graph->neighbours[id]->size; i++) {
		int index = *(int *)curr_node->data;
		if (graph->neighbours[index]->size > graph->neighbours[max]->size) {
			max = index; 
			ok++;
		}
		curr_node = curr_node->next;
	}

	if (ok == 0) {
		printf("%s is the most popular\n", name);
	} else {
		printf("%s is the most popular friend of %s\n", get_user_name(max), name);
	}
}
