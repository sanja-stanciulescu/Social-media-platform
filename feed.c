#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "graph.h"
#include "tree.h"
#include "posts.h"
#include "feed.h"

static linked_list_t *friend_list;

void handle_input_feed(char *input, list_graph_t *graph, linked_list_t *fyp, linked_list_t **profiles)
{
	char *commands = strdup(input);
    if (!commands) {
        return;
    }
	if (!strncmp(commands, "feed", 4)) {
		char name[MAX_NAME_LEN];
        unsigned int feed_size;
        sscanf(commands, "%*s %s %u", name, &feed_size);
        show_feed(fyp, graph, name, feed_size);
    } else if (!strncmp(commands, "view-profile", 12)) {
		char name[MAX_NAME_LEN];
        sscanf(commands, "%*s %s", name);
        view_profile(profiles, name);
    } else if (!strncmp(commands, "friends-repost", 14)) {
		char name[MAX_NAME_LEN];
        int post_id;
        sscanf(commands, "%*s %s %d", name, &post_id);
        friends_repost(fyp, graph, name, post_id);
    } else if (!strncmp(commands, "common-group", 12)) {
		char name[MAX_NAME_LEN];
        sscanf(commands, "%*s %s", name);
        common_group(graph, name);
    }
	free(commands);
}

void show_feed(linked_list_t *fyp, list_graph_t *graph, char *name, unsigned int feed_size) {
    int id = get_user_id(name);
    ll_node_t *curr_node = fyp->head;
    if (feed_size > fyp->size)
        feed_size = fyp->size;

    for (unsigned int i = 0; i < feed_size; i++) {
        if (id == ((post_t *)curr_node->data)->user_id)
            printf("%s: \"%s\"\n", get_user_name(((post_t *)curr_node->data)->user_id), ((post_t *)curr_node->data)->title);
        if (get_node(graph->neighbours[id], &((post_t *)curr_node->data)->user_id, check_users))
            printf("%s: \"%s\"\n", get_user_name(((post_t *)curr_node->data)->user_id), ((post_t *)curr_node->data)->title);
        curr_node = curr_node->next;
    }
}

void view_profile(linked_list_t **profiles, char *name) {
    int id = get_user_id(name);
    ll_node_t *curr = profiles[id]->head;
    for (unsigned int i = 0; i < profiles[id]->size; i++) {
        post_t *post = (post_t *)curr->data;
        if (post->tree) { //este post daca are tree
            printf("Posted: \"%s\"\n", post->title);
        } else {
            printf("Reposted: \"%s\"\n", post->title);
        }
        curr = curr->next;
    }
}

void check_owner(void *ptr) {
    post_t *post = (post_t *)ptr;
    if (get_node(friend_list, &(post->user_id), check_users))
        printf("%s\n", get_user_name(post->user_id));
}

void friends_repost(linked_list_t *fyp, list_graph_t *graph, char *name, int post_id) {
    int id = get_user_id(name);
    friend_list = graph->neighbours[id];
    ll_node_t *wanted_node = get_node(fyp, &post_id, check_post_id);
    post_t *wanted_post = ((post_t *)wanted_node->data);
    dfs(wanted_post->tree->root, 2, check_owner);
}

void clique_selection_sort(linked_list_t *list, int (*comp_func)(void *, void *)) {
    unsigned int n = list->size;
    ll_node_t *curr_node, *next_node;

    curr_node = list->head;
    next_node= NULL;
    if (list->size > 1)
        next_node = curr_node->next;
    for (unsigned int i = 0; i < n - 1; i++) {
        ll_node_t *min_node = curr_node, *j_node = next_node;
        for (unsigned int j = i + 1; j < n; j++) {
            if (comp_func(min_node->data, j_node->data) > 0) {
                min_node = j_node;
            }
            j_node = j_node->next;
        }
        if (min_node != curr_node) {
            int aux = *(int *)curr_node->data;
            memcpy(curr_node->data, min_node->data, sizeof(int));
            memcpy(min_node->data, &aux, sizeof(int));
        }
        curr_node = next_node;
        if (next_node)
            next_node = next_node->next;
    }
}

linked_list_t *intersect(linked_list_t *list1, linked_list_t *list2) {
    linked_list_t *result = ll_create(sizeof(int));
    ll_node_t *curr = list1->head;
    while (curr) {
        if (get_node(list2, curr->data, check_users)) {
            ll_add_nth_node(result, result->size, curr->data);
        }
        curr = curr->next;
    }
    return result;
}

linked_list_t *bronkerbosch(linked_list_t *R, linked_list_t *P, linked_list_t *X, list_graph_t *graph) {
    linked_list_t *max = ll_create(sizeof(int));
    if (P->size == 0 && X->size == 0) {
        ll_node_t *node = R->head;
        while (node) {
            int user = *(int *)node->data;
            ll_add_nth_node(max, max->size, &user);
            node = node->next;
        }
        return max;
    }

    linked_list_t *R_new, *P_new, *X_new;
    ll_node_t *P_node = P->head;
    while (P_node != NULL) {
        int user = *(int *)P_node->data;
        R_new = ll_create(sizeof(int));
        ll_node_t *R_node = R->head;
        while (R_node) {
            int v = *(int *)R_node->data;
            ll_add_nth_node(R_new, R_new->size, &v);
            R_node = R_node->next;
        }
        ll_add_nth_node(R_new, R_new->size, &user);
        P_new = intersect(P, graph->neighbours[user]);
        X_new = intersect(X, graph->neighbours[user]);

        linked_list_t *possible_clique = bronkerbosch(R_new, P_new, X_new, graph);

        if (possible_clique->size > max->size) {
            ll_free(&max, free);
            max = possible_clique;
        } else {
            ll_free(&possible_clique, free);
        }
        ll_free(&R_new, free);
        ll_free(&P_new, free);
        ll_free(&X_new, free);
        R_node = P_node->next;
        unsigned int pos = ll_get_pos(P, P_node, check_users);
        ll_node_t *removed_node = ll_remove_nth_node(P, pos);
        free(removed_node->data);
        free(removed_node);
        ll_add_nth_node(X, X->size, &user);
        P_node = R_node;
    }
    return max;
}

void common_group(list_graph_t *graph, char *name) {
    int id = get_user_id(name);
    if (graph->neighbours[id]->size == 0) {
        printf("The closest friend group of %s is:\n", name);
        printf("%s\n", name);
    } else {
        linked_list_t *R, *P, *X;
        R = ll_create(sizeof(int));
        P = ll_create(sizeof(int));
        X = ll_create(sizeof(int));
        //initializes P with all the graph nodes that are neighbours
        ll_add_nth_node(P, P->size, &id);
        ll_node_t *curr = graph->neighbours[id]->head;
        for (unsigned int i = 0; i < graph->neighbours[id]->size; i++) {
            int user = *(int *)curr->data;
            ll_add_nth_node(P, P->size, &user);
            curr = curr->next;
        }
        //initializes R with the starting node (the one that has name)
        //ll_add_nth_node(R, R->size, &id);

        linked_list_t *clique = bronkerbosch(R, P, X, graph);
        clique_selection_sort(clique, check_users);
        curr = clique->head;
        printf("The closest friend group of %s is:\n", name);
	    while (curr) {
		    printf("%s\n", get_user_name(*((int*)curr->data)));
		    curr = curr->next;
	    }

        ll_free(&P, free);
        ll_free(&X, free);
        ll_free(&R, free);
        ll_free(&clique, free);
    }
}
