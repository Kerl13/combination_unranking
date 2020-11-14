#include <gmp.h>
#include <string.h> // bzero
#include <malloc.h>
#include <assert.h>

#include "../combunrank.h"


void factoradic_decomp(int* dest, int len, const mpz_t u) {
  bzero(dest, sizeof(int) * len);

  if (mpz_sgn(u) == 0) return;

  // Invariant: fact = n!
  int n = len - 1;
  mpz_t fact;
  mpz_init(fact);
  mpz_fac_ui(fact, n);

  // Find the unique n such that: n! ≤ u < (n + 1)!
  while (mpz_cmp(fact, u) > 0) {
    mpz_divexact_ui(fact, fact, n);
    n--;
  }

  mpz_t f_n;
  mpz_init(f_n);
  // Invariant: u = x + \sum_{i=n+1}^{len-1} dest[i] * i!
  mpz_t x;
  mpz_init_set(x, u);
  while (mpz_sgn(x) > 0) {
    mpz_tdiv_qr(f_n, x, x, fact);
    dest[n] = mpz_get_si(f_n);
    mpz_divexact_ui(fact, fact, n);
    n--;
  }
}

void factoradic_recomp(mpz_t dest, int* dec, int len) {
  mpz_t fac;
  mpz_init_set_ui(fac, 1);

  mpz_set_ui(dest, 0);

  for (int i = 1; i < len; i++) {
    mpz_mul_ui(fac, fac, i);
    mpz_addmul_ui(dest, fac, dec[i]);
  }

  mpz_clear(fac);
}


// Data structure with O(ln k) access to the k-th element: interval tree
typedef enum tree_type { INTERVAL, NODE } tree_type;
typedef struct interval_s { int min; int max; } interval;
typedef struct node_s { struct tree_s* left; struct tree_s* right; } node;
typedef struct tree_s {
  tree_type type;
  int size;
  union { interval as_interval; node as_node; } content;
} tree;

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

void extract(int* dest, const int* F, int n, int k) {
  tree* P = tree_make(0, n - 1);
  for (int i = 0; i < k; i++) {
    dest[i] = tree_pop(P, F[n - 1 - i]);
  }
}

void unrank_permutation(int* dest, int n, const mpz_t rank) {
  int* F = calloc(n, sizeof(int));
  factoradic_decomp(F, n, rank);
  extract(dest, F, n, n);
  free(F);
}

void unrank_factoradics_fast(int* dest, int n, int k, const mpz_t rank) {
  mpz_t binom, r;
  mpz_init_set(r, rank);
  mpz_init(binom);
  mpz_bin_uiui(binom, n - 1, k - 1);
  mpz_mul_ui(binom, binom, n);

  int m = 0;
  int i = 0;

  while (i < k) {
    mpz_divexact_ui(binom, binom, n - m);
    if (mpz_cmp(binom, r) > 0) {
      dest[i] = m + i;
      mpz_mul_ui(binom, binom, k - i - 1);
      i++;
      n--;
    } else {
      mpz_sub(r, r, binom);
      mpz_mul_ui(binom, binom, n - m - k + i);
      m++;
    }
  }
}
