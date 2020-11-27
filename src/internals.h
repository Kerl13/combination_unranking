#ifndef _COMBUNRANK_INTERNALS_H
#define _COMBUNRANK_INTERNALS_H

#include <stdio.h>

// Data structure with O(ln k) access to the k-th element: interval tree
typedef enum tree_type { INTERVAL, NODE } tree_type;
typedef struct interval_s { int min; int max; } interval;
typedef struct node_s { struct tree_s* left; struct tree_s* right; } node;
typedef struct tree_s {
  tree_type type;
  int size;
  union { interval as_interval; node as_node; } content;
} tree;

tree* tree_make(int min, int max);
void tree_clear(tree*);
void print_tree(FILE*, const tree*);
void tree_print(FILE*, const tree*);
int tree_pop(tree*, int);

#endif
