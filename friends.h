#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550
#define MAX_NAME_LEN 100

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(char *input, list_graph_t *graph);

/**
 * Function that creates a link between to users
*/
void add_friends(list_graph_t *graph, char *name_1, char *name_2);

void remove_friends(list_graph_t *graph, char *name_1, char *name_2);

void distance_friends(list_graph_t *graph, char *name_1, char *name_2);

void friend_suggestions(list_graph_t *graph, char *name);

void common_friends(list_graph_t *graph, char *name_1, char *name_2);

void friends_number(list_graph_t *graph, char *name);

void popular_friend(list_graph_t *graph, char *name);

#endif // FRIENDS_H
