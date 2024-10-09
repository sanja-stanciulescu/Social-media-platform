#ifndef TREE_H
#define TREE_H

#define MAX_N_CHILDREN 100
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define INF 999

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

typedef struct tree_node_t tree_node_t;
typedef struct events_tree_t events_tree_t;

struct tree_node_t {
    void *data;
    int n_children;
    tree_node_t **children;
    void (*free_func)(void *);
};

struct events_tree_t {
    tree_node_t *root;
    unsigned int data_size;
};

tree_node_t *tree_node_create(void *data, size_t data_size, void (*free_func)(void *));

events_tree_t *events_tree_create(size_t data_size);

tree_node_t *break_off_father(tree_node_t *root, void *data, int (*cmp_func)(void *, void *));

tree_node_t *find_tree_node(tree_node_t *root, void *data, int (*cmp_func)(void *, void *));

void add_child_node(tree_node_t *parent, void *data, size_t data_size, void (*free_func)(void *));

void free_tree_node(tree_node_t *node);

void free_tree(events_tree_t *tree);

void dfs(tree_node_t *node, int ok, void (*function)(void *));

int is_node_in_tree(tree_node_t *root, void *data, int (*cmp_func)(void *, void *));

tree_node_t *find_lca(tree_node_t *root, void *data1, void *data2, int (*cmp_func)(void *, void *));

#endif //TREE_H