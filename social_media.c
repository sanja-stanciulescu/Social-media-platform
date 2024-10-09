/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "graph.h"
#include "tree.h"
#include "posts.h"
#include "friends.h"
#include "feed.h"

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();

	#ifdef TASK_1
    list_graph_t *graph;
	graph = lg_create(MAX_PEOPLE);
	#endif

	#ifdef TASK_2
    linked_list_t *fyp;
	fyp = ll_create(sizeof(post_t));
    linked_list_t **profiles = malloc(MAX_PEOPLE * sizeof(linked_list_t *));
    for (int i = 0; i < MAX_PEOPLE; i++) {
        profiles[i] = ll_create(sizeof(post_t));
    }
	#endif

	#ifdef TASK_3

	#endif

	char *input = (char *)malloc(MAX_COMMAND_LEN);

	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, graph);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, fyp, profiles);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, graph, fyp, profiles);
		#endif
	}

    #ifdef TASK_1
    lg_free(graph);
    #endif

    #ifdef TASK_2
    for (int i = 0; i < MAX_PEOPLE; i++) {
        ll_node_t* currNode;
	
	    while (ll_get_size(profiles[i]) > 0) {
		    currNode = ll_remove_nth_node(profiles[i], 0);
		    free_post(currNode->data);
		    currNode->data = NULL;
		    free(currNode);
		    currNode = NULL;
	    }
	    free(profiles[i]);
    }
    free(profiles);

    ll_node_t* currNode;
	
	while (ll_get_size(fyp) > 0) {
		currNode = ll_remove_nth_node(fyp, 0);
		free_post(currNode->data);
		currNode->data = NULL;
		free(currNode);
		currNode = NULL;
	}
	free(fyp);
    #endif

    #ifdef TASK_3

    #endif
	free_users();
	free(input);

	return 0;
}
