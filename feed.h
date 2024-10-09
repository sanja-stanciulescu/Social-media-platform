#ifndef FEED_H
#define FEED_H

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_feed(char *input, list_graph_t *graph, linked_list_t *fyp, linked_list_t **profiles);

void show_feed(linked_list_t *fyp, list_graph_t *graph, char *name, unsigned int feed_size);

void view_profile(linked_list_t **profiles, char *name);

void check_owner(void *ptr);

void friends_repost(linked_list_t *fyp, list_graph_t *graph, char *name, int post_id);

void clique_selection_sort(linked_list_t *list, int (*comp_func)(void *, void *));

linked_list_t *intersect(linked_list_t *list1, linked_list_t *list2);

linked_list_t *bronkerbosch(linked_list_t *R, linked_list_t *P, linked_list_t *X, list_graph_t *graph);

void common_group(list_graph_t *graph, char *name);

#endif // FEED_H
