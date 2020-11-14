#include <malloc.h>
#include <stdio.h>
#include <assert.h>

#include "internals.h"


tree* tree_make(int min, int max) {
  tree* t = malloc(sizeof(tree));
  t->type = INTERVAL;
  t->size = max - min + 1;
  t->content.as_interval.min = min;
  t->content.as_interval.max = max;
  return t;
}

void tree_clear(tree* t) {
  if (t->type == NODE) {
    tree_clear(t->content.as_node.left);
    tree_clear(t->content.as_node.right);
  }
  free(t);
}

void tree_print(FILE* fd, const tree* t) {
  switch (t->type) {
    case INTERVAL:
      fprintf(fd, "(%d)[%d; %d]",
              t->size,
              t->content.as_interval.min,
              t->content.as_interval.max);
      break;
    case NODE:
      fprintf(fd, "Node(%d, ", t->size);
      tree_print(fd, t->content.as_node.left);
      fprintf(fd, ", ");
      tree_print(fd, t->content.as_node.right);
      fprintf(fd, ")");
      break;
  }
}

int tree_pop(tree* t, int i) {
  assert(i >= 0);
  assert(i < t->size);

  switch (t->type) {
    case INTERVAL: {
      const int res = t->content.as_interval.min + i;
      if (i == 0) t->content.as_interval.min++;
      else if (i == t->size - 1) t->content.as_interval.max--;
      else {
        const int min = t->content.as_interval.min;
        const int max = t->content.as_interval.max;
        t->type = NODE;
        t->content.as_node.left = tree_make(min, min + i - 1);
        t->content.as_node.right = tree_make(min + i + 1, max);
      }
      t->size--;
      return res;
    }
    case NODE: {
      const int lsize = t->content.as_node.left->size;
      t->size--;
      if (i < lsize) {
        return tree_pop(t->content.as_node.left, i);
      } else {
        return tree_pop(t->content.as_node.right, i - lsize);
      }
    }
    default:
      return -1;
  }
}
