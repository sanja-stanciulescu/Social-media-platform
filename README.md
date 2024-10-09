# Social Media Platform

### Description:

Team project () that consisted of implementing from scratch a social media platform using multiple data structures.
The main functionalities of the platform are:

# Task 1 (Friendship Network)

* To track friendships between users, a graph implemented with lists is used. This implementation was chosen because using a matrix would not have been efficient.
* The functions 'Add Friend' and 'Remove Friend' simply involve adding or removing links between nodes in this graph.
* For 'Find Distance', the shortest path between two nodes is found using BFS. Two vectors, visited and distance, are created to keep track of the visited nodes and the distance between the source node and the other nodes.
* 'Suggestions' performs a BFS search only for the neighbors of the source node, resulting in a list containing the friends of friends who are not already friends with a given user.
* In 'Common Friends', each friend from the first user's list is checked to see if they also exist in the second user's list.
* 'Number of Friends' returns the number of connections a person has, using the graph implemented with lists. The number of connections represents the number of neighbors a node has.
* In 'Most Popular', the function traverses all neighbors of a given node, looking for the neighbor (user) with the highest number of connections. Initially, the most popular user is the node given as a parameter, but this changes if another node has more neighbors.

# Task 2 (Posts and Reposts)

* In 'Create a Post', a new post is created for the specified user and is added both to the main feed and to the user's profile. A perfect copy of the post is also created, with its pointer to the tree set to NULL. This represents the root of the tree within the post structure.
* 'Repost' involves creating a repost of an existing post. The repost can be made on the original post or on one of the existing reposts. A copy of the repost is added to the user's profile, and the repost is added to the event tree of the original post.
* 'First Common Repost' locates the 'first common ancestor' between two reposts in the event tree of the original post and displays its ID.
* 'Like' allows a user to like a post or repost. If the user has already liked the post, the like will be removed (unlike). Likes are stored in a singly linked list, and to determine if a user has already liked a post, this list is traversed to find their ID.
* The 'Ratio' function works by checking if a repost has more likes than the original post. First, the repost with the highest number of likes is found, and if it has more likes than the original post, the repost is considered 'Ratio'd'. If the repost with the maximum likes has fewer than the original post, the repost_id remains at -1, indicating that no Ratio occurred.
* In 'Delete Post/Repost', a specified post or repost is deleted from the feed and the corresponding structures.
* The 'Get likes' function displays the size of the singly linked list corresponding to the likes received on a certain post.
* To 'Get reposts', the function starts from an initial post or specified repost and traverses the corresponding tree or subtree. The print_repost_data function is used to display the information of a repost.

# Task 3 (Feed)

* In this task, functions from Task 1 and Task 2 are used to create the friendship and post network, i.e., a feed.
* The 'handle_input_feed' function serves only to parse the instructions from the command line and call the corresponding functions.
* To 'Show the Feed', the function accesses the singly linked list corresponding to the main page (fyp), which is already sorted by the most recent posts. It checks whether the post was made by the user <name> or by one of their friends (the adjacency list in the graph is checked).
* To 'View Profile', the function accesses the vector of singly linked lists (<profiles>) created specifically for this function. The user ID <name> is obtained, and all posts and reposts are displayed.
* For 'Friends' Reposts', the function traverses the tree associated with <post_id> and checks which repost was made by a friend of the user <name> through DFS. This algorithm is linked with the 'check_owner' function to compare the <user_id> associated with the repost and the nodes in the adjacency list of friends.
* For 'Common Group', the suggested Bron-Kerbosch algorithm is used. If the user <name> has no friends, the algorithm does not need to be called. Otherwise, lists R (to hold the returned clique), P (list of possible nodes), and X (visited nodes) are initialized. All nodes from the adjacency list of friends are copied into P, forcing the algorithm to compute a maximal clique associated with the user passed as a parameter. After calling the 'bronkerbosch' function, the result is a clique that is not sorted by ID. Therefore, the 'clique_selection_sort' function is applied to this list, which works exactly like selection sort, but handles lists instead of vectors.
* More details about Bron-Kerbosch: If both P and X are empty, R is a maximal clique and can be returned. Since we want the largest clique, after R is returned, its output must be compared with the other recursive outputs to ensure it's the largest clique. Finally, the maximum found is returned. The rest of the algorithm works the same as in the pseudocode from Wikipedia.

# (Optional) Resources / Bibliography:
(https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm#Without_pivoting)
