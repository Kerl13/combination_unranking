#include <malloc.h>
#include <assert.h>
#include <gmp.h>
#include "../combunrank.h"

static int is_smaller(const int* p1, const int* p2, int n) {
  for (int i = 0; i < n; i++) {
    if (p1[i] < p2[i]) return 1;
    else if (p1[i] > p2[i]) return 0;
  }
  return 0;
}

static int is_valid(const int* p, int n) {
  for (int i = 0; i < n; i++) {
    if (p[i] < 0 || p[i] >= n) return 0;
  }
  return 1;
}

static int test(int n) {
  mpz_t rank, fac;
  mpz_init(rank);
  mpz_init(fac);
  mpz_fac_ui(fac, n);

  int* p1 = calloc(n, sizeof(int));

  // The first permutation is [0, 1, 2, …, n-1]
  unrank_permutation(p1, n, rank);
  for (int i = 0; i < n; i++) assert(p1[i] == i);

  int* p2 = calloc(n, sizeof(int));
  for (mpz_set_ui(rank, 1); mpz_cmp(rank, fac) < 0; mpz_add_ui(rank, rank, 1)) {
    // Reset p2, just in case
    for (int i = 0; i < n; i++) p2[i] = 0;

    unrank_permutation(p2, n, rank);
    if (! (is_smaller(p1, p2, n) && is_valid(p2, n))) {
      return 1;
    }

    int* tmp = p1;
    p1 = p2;
    p2 = tmp;
  }

  return 0;
}

int main() {
  int N = 8;
  for (int n = 1; n <= N; n++) {
    assert(test(n) == 0);
  }

  printf("%s  OK\n", "↪");
  return 0;
}
