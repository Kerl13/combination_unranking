#include <gmp.h>
#include <malloc.h>
#include <stdio.h>
#include <assert.h>

#include "../combunrank.h"


// The type of combination unranking algorithms
typedef void (*unrank_algo)(int*, int, int, const mpz_t);

int is_valid_combination(const int* combi, int n, int k) {
  for (int i = 0; i < k - 1; i++) {
    if (combi[i] >= combi[i + 1]) return 0;
  }
  return (combi[0] >= 0) && (combi[k - 1] < n);
}

int is_smaller(const int* t1, const int* t2, int length) {
  for (int i = 0; i < length; i++) {
    if (t1[i] < t2[i]) return 1;
    if (t1[i] > t2[i]) return 0;
  }

  // The arrays are equal
  return 0;
}

static int check_all(int n, int k, unrank_algo f) {
  int* t1 = calloc(k, sizeof(int));
  int* t2 = calloc(k, sizeof(int));

  mpz_t rank, binom;
  mpz_init_set_ui(rank, 0);
  mpz_init(binom);
  mpz_bin_uiui(binom, n, k);

  f(t1, n, k, rank);
  assert(is_valid_combination(t1, n, k));
  mpz_add_ui(rank, rank, 1);

  while (mpz_cmp(rank, binom) < 0) {
    // Reset t2
    for (int i = 0; i < k; i++) t2[i] = 0;

    // Get and check the next combination
    f(t2, n, k, rank);
    if (! is_valid_combination(t2, n, k) || ! is_smaller(t1, t2, k)) {
      fprintf(stderr, "Test failed at n=%d k=%d rank=", n, k);
      mpz_out_str(stderr, 10, rank);
      fprintf(stderr, "\nt1 = {");
      for (int i = 0; i < k; i++) fprintf(stderr, "%d, ", t1[i]);
      fprintf(stderr, "}\nt2 = {");
      for (int i = 0; i < k; i++) fprintf(stderr, "%d, ", t2[i]);
      fprintf(stderr, "}\n");
      fflush(stderr);
      return 1;
    }

    // Swap the pointers
    int* tmp = t1;
    t1 = t2;
    t2 = tmp;

    // Move on to the next rank
    mpz_add_ui(rank, rank, 1);
  }

  free(t1);
  free(t2);

  return 0;
}

typedef void (*algo)(int*, int, int, const mpz_t);
typedef struct pair {
  char* name;
  algo f;
} pair;

static const pair algos[] = {
  {.name = "recursive_method", .f = unrank_recursive_method},
  {.name = "recursive_method_naive", .f = unrank_recursive_method_naive},
  {.name = "factoradics", .f = unrank_factoradics_fast},
};

int main() {
  int N = 15;

  for (unsigned int i = 0; i < sizeof(algos) / sizeof(pair); i++) {
    algo algo = algos[i].f;
    printf("Testing %s…\n", algos[i].name);
    for (int n = 1; n <= N; n++) {
      for (int k = 1; k <= n; k++) {
        assert(check_all(n, k, algo) == 0);
      }
    }
  }

  return 0;
}
