#ifndef POSTS_H
#define POSTS_H

#define MAX_COMMAND_LEN 500
#define MAX_TITLE_LEN 285
#define MAX_NAME_LEN 100

#define DIE(assertion, call_description)            \
	do                                              \
	{                                               \
		if (assertion)                              \
		{                                           \
			fprintf(stderr, "(%s, %d): ", __FILE__, \
					__LINE__);                      \
			perror(call_description);               \
			exit(errno);                            \
		}                                           \
	} while (0)

typedef struct post_t post_t;

struct post_t {
    int id;
    char title[MAX_TITLE_LEN];
    int user_id;
    linked_list_t *likes;
    events_tree_t *tree;
};

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_posts(char *input, linked_list_t *fyp, linked_list_t **profiles);

/**
 * This function frees a post_t struct;
*/
void free_post(void *ptr);

void create_post(linked_list_t *fyp, linked_list_t **profiles, char *name, char *title);

void repost_to_post(linked_list_t *fyp, linked_list_t **profiles, char *name, int post_id, int repost_id);

int check_post_id(void *post, void *id);

void print_repost_data(void *data);

void get_reposts(linked_list_t *fyp, int post_id, int repost_id);

void common_repost(linked_list_t *fyp, int post_id, int repost_id1, int repost_id2);

int check_users(void *id1, void *id2);

void like_post(linked_list_t *fyp, char *name, int post_id, int repost_id);

void get_likes(linked_list_t *fyp, int post_id, int repost_id);

void get_max_likes(tree_node_t *node, unsigned int *max, int *repost_id);

void get_ratio(linked_list_t *fyp, int post_id);

void delete_post(linked_list_t *fyp, int post_id, int repost_id);

#endif // POSTS_H
