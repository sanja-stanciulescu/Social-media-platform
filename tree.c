#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "graph.h"
#include "users.h"
#include "tree.h"

tree_node_t *tree_node_create(void *data, size_t data_size, void (*free_func)(void *)) {
    tree_node_t *node;
    node = malloc(sizeof(tree_node_t));
    DIE(node == NULL, "tree_node malloc");

    node->data = malloc(data_size);
    DIE(node->data == NULL, "node->data malloc");
    memcpy(node->data, data, data_size);

    node->n_children = 0;
    node->children = NULL;
    node->free_func = free_func;
    
    return node;
}

events_tree_t *events_tree_create(size_t data_size) {
    events_tree_t *tree;

    tree = malloc(sizeof(events_tree_t));
    DIE(tree == NULL, "tree malloc");

    tree->root = NULL;
    tree->data_size = data_size;

    return tree;
}

tree_node_t *break_off_father(tree_node_t *root, void *data, int (*cmp_func)(void *, void *)) {
    if (!root)
        return NULL;

    if(cmp_func(root->data, data) == 0)
        return root;

    for (int i = 0; i < root->n_children; i++) {
        tree_node_t *result = break_off_father(root->children[i], data, cmp_func);
        if (result == root->children[i])
            root->children[i] = NULL;
        if (result)
            return result;
    }
    return NULL;
}

tree_node_t *find_tree_node(tree_node_t *root, void *data, int (*cmp_func)(void *, void *)) {
    if (!root)
        return NULL;

    if(cmp_func(root->data, data) == 0)
        return root;

    for (int i = 0; i < root->n_children; i++) {
        tree_node_t *result = find_tree_node(root->children[i], data, cmp_func);
        if (result)
            return result;
    }
    return NULL;
}

void add_child_node(tree_node_t *parent, void *data, size_t data_size, void (*free_func)(void *)) {
    if (parent->children == NULL) {
        parent->children = calloc(MAX_N_CHILDREN, sizeof(tree_node_t *));
    }
    DIE(parent->children == NULL, "tree_node_t->children malloc");

    tree_node_t *child = tree_node_create(data, data_size, free_func);
    parent->children[parent->n_children] = child;
    parent->n_children++;
}

void free_tree_node(tree_node_t *node) {
    if (!node)
        return;
    
    for (int i = 0; i < node->n_children; i++) {
        free_tree_node(node->children[i]);
        if (i != 0)
            node->children[i] = NULL;
    }

    if (node->free_func) {
        node->free_func(node->data);
    } else {
        free(node->data);
    }
    free(node->children);
    free(node);
}

void free_tree(events_tree_t *tree) {
    if (!tree)
        return;
    if (tree->root)
        free_tree_node(tree->root);
    free(tree);
    tree = NULL;
}

void dfs(tree_node_t *node, int ok, void (*function)(void *)) {
    if (node == NULL)
        return;
    if (ok != 1)
        function(node->data);

    for (int i = 0; i < node->n_children; i++) {
        ok++;
        dfs(node->children[i], ok, function);
    }
}

int is_node_in_tree(tree_node_t *root, void *data, int (*cmp_func)(void *, void *)) {
    if (!root)
        return 0;

    if (cmp_func(root->data, data) == 0)
        return 1;
    
    for (int i = 0; i < root->n_children; i++) {
        if (is_node_in_tree(root->children[i], data, cmp_func))
            return 1;
    }
    return 0;
}

tree_node_t *find_lca(tree_node_t *root, void *data1, void *data2, int (*cmp_func)(void *, void *)) {
    if (!root)
        return NULL;

    if (cmp_func(root->data, data1) == 0 || cmp_func(root->data, data2) == 0)
        return root;

    //int found_in_left = 0, found_in_right = 0;
    tree_node_t *lca = NULL;

    for (int i = 0; i < root->n_children; i++) {
        tree_node_t *temp_lca = find_lca(root->children[i], data1, data2, cmp_func);
        if (temp_lca != NULL) {
            if (lca != NULL)
                return root;
            lca = temp_lca;
        }
    }

    return lca;
}