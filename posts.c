#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "graph.h"
#include "tree.h"
#include "posts.h"

static int idx = 0;

void handle_input_posts(char *input, linked_list_t *fyp, linked_list_t **profiles)
{
	char *commands = strdup(input);
	if (!commands)
		return;

	if (!strncmp(commands, "create", 6)) {
		char name[MAX_NAME_LEN], title[MAX_TITLE_LEN];
        sscanf(commands, "%*s %s \"%280[^\"]\"", name, title);
        idx++;
        create_post(fyp, profiles, name, title);
    } else if (!strncmp(commands, "repost", 6)) {
		char name[MAX_NAME_LEN];
        int post_id, repost_id;
        idx++;
        if (sscanf(commands, "%*s %s %d %d\n", name, &post_id, &repost_id) != 3)
            repost_id = -1;
        repost_to_post(fyp, profiles, name, post_id, repost_id);
    } else if (!strncmp(commands, "common-repost", 13)) {
		int post_id, repost_id1, repost_id2;
        sscanf(commands, "%*s %d %d %d\n", &post_id, &repost_id1, &repost_id2);
        common_repost(fyp, post_id, repost_id1, repost_id2);
    } else if (!strncmp(commands, "like", 4)) {
		char name[MAX_NAME_LEN];
        int post_id, repost_id;
        if (sscanf(commands, "%*s %s %d %d", name, &post_id, &repost_id) != 3)
            repost_id = -1;
        like_post(fyp, name, post_id, repost_id);
    } else if (!strncmp(commands, "ratio", 5)) {
		int post_id;
        sscanf(commands, "%*s %d", &post_id);
        get_ratio(fyp, post_id);
    } else if (!strncmp(commands, "delete", 6)) {
		int post_id, repost_id;
        if (sscanf(commands, "%*s %d %d\n", &post_id, &repost_id) != 2)
            repost_id = -1;
        delete_post(fyp, post_id, repost_id);
	} else if (!strncmp(commands, "get-likes", 9)) {
		int post_id, repost_id;
        if (sscanf(commands, "%*s %d %d\n", &post_id, &repost_id) != 2)
            repost_id = -1;
        get_likes(fyp, post_id, repost_id);
	} else if (!strncmp(commands, "get-reposts", 11)) {
		int post_id, repost_id;
        if (sscanf(commands, "%*s %d %d\n", &post_id, &repost_id) != 2)
            repost_id = -1;
        get_reposts(fyp, post_id, repost_id);
    }

	free(commands);
}

void free_post(void *ptr) {
    post_t *post_ptr = (post_t *)ptr;
    if (post_ptr->tree) {
        free_tree(post_ptr->tree);
    }
    ll_free(&post_ptr->likes, free);
    free(post_ptr);
    post_ptr = NULL;
}

void create_post(linked_list_t *fyp, linked_list_t **profiles, char *name, char *title) {
    post_t *new_post = malloc(sizeof(post_t));
    post_t *carbon_copy = malloc(sizeof(post_t));
    post_t *profile_copy = malloc(sizeof(post_t));

    new_post->id = idx;
    carbon_copy->id = idx;
    profile_copy->id = idx;

    new_post->user_id = get_user_id(name);
    carbon_copy->user_id = new_post->user_id;
    profile_copy->user_id = new_post->user_id;

    memcpy(new_post->title, title, MAX_TITLE_LEN);
    memcpy(carbon_copy->title, title, MAX_TITLE_LEN);
    memcpy(profile_copy->title, title, MAX_TITLE_LEN);

    new_post->likes = ll_create(sizeof(int));
    carbon_copy->likes = ll_create(sizeof(int));
    profile_copy->likes = ll_create(sizeof(int));

    new_post->tree = events_tree_create(sizeof(post_t));
    carbon_copy->tree = NULL;
    profile_copy->tree = events_tree_create(sizeof(post_t));
    new_post->tree->root = tree_node_create(carbon_copy, sizeof(post_t), free_post);

    ll_add_nth_node(fyp, 0, new_post);
    ll_add_nth_node(profiles[new_post->user_id], profiles[new_post->user_id]->size, profile_copy);
    printf("Created \"%s\" for %s\n", title, name);
    free(new_post);
    free(carbon_copy);
    free(profile_copy);
}

int check_post_id(void *post, void *id) {
    post_t *post_data = (post_t *)post;
    int right_id = *(int *)id;

    if (post_data->id == right_id)
        return 0;
    return 1;
}

void repost_to_post(linked_list_t *fyp, linked_list_t **profiles, char *name, int post_id, int repost_id) {
    post_t *new_repost = malloc(sizeof(post_t));
    post_t *profile_copy = malloc(sizeof(post_t));

    new_repost->id = idx;
    profile_copy->id = idx;

    new_repost->tree = NULL;
    profile_copy->tree = NULL;

    new_repost->user_id = get_user_id(name);
    profile_copy->user_id = new_repost->user_id;

    new_repost->likes = ll_create(sizeof(int));
    profile_copy->likes = ll_create(sizeof(int));

    ll_node_t *wanted_node = get_node(fyp, &post_id, check_post_id);
    post_t *wanted_post = ((post_t *)wanted_node->data);

    memcpy(profile_copy->title, wanted_post->title, MAX_TITLE_LEN);

    ll_add_nth_node(profiles[new_repost->user_id], profiles[new_repost->user_id]->size, profile_copy);
    if (repost_id == -1) {
        add_child_node(wanted_post->tree->root, new_repost, sizeof(post_t), free_post);
    } else {
        tree_node_t *wanted_repost = find_tree_node(wanted_post->tree->root, &repost_id, check_post_id);
        add_child_node(wanted_repost, new_repost, sizeof(post_t), free_post);
    }

    printf("Created repost #%d for %s\n", idx, name);
    free(new_repost);
    free(profile_copy);
}

void print_repost_data(void *data) {
    post_t *repost = (post_t *)data;
    printf("Repost #%d by %s\n", repost->id, get_user_name(repost->user_id));
}

void get_reposts(linked_list_t *fyp, int post_id, int repost_id) {
    ll_node_t *wanted_node = get_node(fyp, &post_id, check_post_id);
    post_t *wanted_post = ((post_t *)wanted_node->data);
    if (repost_id == -1) {
        printf("\"%s\" - Post by %s\n", wanted_post->title, get_user_name(wanted_post->user_id));
        int ok = 1;
        dfs(wanted_post->tree->root, ok, print_repost_data);
    } else {
        int ok = 2;
        tree_node_t *wanted_repost = find_tree_node(wanted_post->tree->root, &repost_id, check_post_id);
        dfs(wanted_repost, ok, print_repost_data);
    }
}

void common_repost(linked_list_t *fyp, int post_id, int repost_id1, int repost_id2) {
    ll_node_t *wanted_node = get_node(fyp, &post_id, check_post_id);
    post_t *wanted_post = ((post_t *)wanted_node->data);
    tree_node_t *wanted_repost = find_lca(wanted_post->tree->root, &repost_id1, &repost_id2, check_post_id);
    printf("The first common repost of %d and %d is %d\n", repost_id1, repost_id2, ((post_t *)wanted_repost->data)->id);
}

int check_users(void *id1, void *id2) {
    int user1 = *(int *)id1, user2 = *(int *)id2;
    if (user1 == user2)
        return 0;
    if (user1 < user2)
        return -1;
    return 1;
}

void like_post(linked_list_t *fyp, char *name, int post_id, int repost_id) {
    int user_id = get_user_id(name);
    ll_node_t *wanted_node = get_node(fyp, &post_id, check_post_id);
    post_t *wanted_post = ((post_t *)wanted_node->data);
    if (repost_id == -1) {
        ll_node_t *liker = get_node(wanted_post->likes, &user_id, check_users);
        if (!liker) {
            ll_add_nth_node(wanted_post->likes, 0, &user_id);
            printf("User %s liked post \"%s\"\n", name, wanted_post->title);
        } else {
            unsigned int n = ll_get_pos(wanted_post->likes, &user_id, check_users);
            ll_node_t *removed_node = ll_remove_nth_node(wanted_post->likes, n);
            free(removed_node->data);
            free(removed_node);
            printf("User %s unliked post \"%s\"\n", name, wanted_post->title);
        }
    } else {
        tree_node_t *wanted_repost = find_tree_node(wanted_post->tree->root, &repost_id, check_post_id);
        post_t *repost = (post_t *)wanted_repost->data;
        ll_node_t *liker = get_node(repost->likes, &user_id, check_users);
        if (!liker) {
            ll_add_nth_node(repost->likes, 0, &user_id);
            printf("User %s liked repost \"%s\"\n", name, wanted_post->title);
        } else {
            unsigned int n = ll_get_pos(repost->likes, &user_id, check_users);
            ll_node_t *removed_node = ll_remove_nth_node(repost->likes, n);
            free(removed_node->data);
            free(removed_node);
            printf("User %s unliked repost \"%s\"\n", name, wanted_post->title);
        }
    }
}

void get_likes(linked_list_t *fyp, int post_id, int repost_id) {
    ll_node_t *wanted_node = get_node(fyp, &post_id, check_post_id);
    post_t *wanted_post = ((post_t *)wanted_node->data);
    if (repost_id == -1) {
        printf("Post \"%s\" has %d likes\n", wanted_post->title, wanted_post->likes->size);
    } else {
        tree_node_t *wanted_repost = find_tree_node(wanted_post->tree->root, &repost_id, check_post_id);
        post_t *repost = (post_t *)wanted_repost->data;
        printf("Repost #%d has %d likes\n", repost_id, repost->likes->size);
    }
}

void get_max_likes(tree_node_t *node, unsigned int *max, int *repost_id) {
    if (node == NULL)
        return;

    if (((post_t *)node->data)->likes->size >= *max) {
        if (((post_t *)node->data)->likes->size == *max) {
            if (((post_t *)node->data)->id < (*repost_id)) {
                *repost_id = ((post_t *)node->data)->id;
            }
        } else {
            *max = ((post_t *)node->data)->likes->size;
            *repost_id = ((post_t *)node->data)->id;
        }
    }

    for (int i = 0; i < node->n_children; i++) {
        get_max_likes(node->children[i], max, repost_id);
    }
}

void get_ratio(linked_list_t *fyp, int post_id) {
    ll_node_t *wanted_node = get_node(fyp, &post_id, check_post_id);
    post_t *wanted_post = ((post_t *)wanted_node->data);
    unsigned int max = wanted_post->likes->size;
    int repost_id = -1;
    get_max_likes(wanted_post->tree->root, &max, &repost_id);
    if (repost_id == -1) {
        printf("The original post is the highest rated\n");
    } else {
        printf("Post %d got ratio'd by repost %d\n", post_id, repost_id);
    }
}

void delete_post(linked_list_t *fyp, int post_id, int repost_id) {
    ll_node_t *wanted_node = get_node(fyp, &post_id, check_post_id);
    post_t *wanted_post = ((post_t *)wanted_node->data);
    if (repost_id == -1) {
        unsigned int n = ll_get_pos(fyp, &post_id, check_post_id);
        ll_node_t *removed_node = ll_remove_nth_node(fyp, n);
        free_post(removed_node);
        printf("Deleted post \"%s\"\n", wanted_post->title);
    } else {
        tree_node_t *wanted_repost = break_off_father(wanted_post->tree->root, &repost_id, check_post_id);
        free_tree_node(wanted_repost);
        wanted_repost = NULL;
        printf("Deleted repost #%d of post \"%s\"\n", repost_id, wanted_post->title);
    }
}