#include <stdio.h>
#include <malloc.h>
#include <time.h> // clock, clock_t, CLOCKS_PER_SEC
#include <string.h> // strcmp
#include <gmp.h>
#include <assert.h>

#include "utils.h"
#include "../combunrank.h"

void unrank_recursive_method_memo(int* dest, int n, int k, const mpz_t rank);
void unrank_combinadics_memo(int* dest, int n, int k, const mpz_t rank);
void unrank_combinadics2_memo(int* dest, int n, int k, const mpz_t rank);
void unrank_factoradics_memo(int* dest, int n, int k, const mpz_t rank);

struct coeff {
  int n;
  int k;
  mpz_t val;
};
enum mode { SET, GET };

static struct coeff * bin_table = NULL;
static size_t memo_capacity = 0;
static size_t memo_size = 0;
static enum mode memoisation_mode = SET;

static void realloc_bin_table(size_t c) {
  const size_t old_cap = memo_capacity;

  if (old_cap > c) {
    for (size_t i = c; i < old_cap; i++) mpz_clear(bin_table[i].val);
  }

  bin_table = reallocarray(bin_table, c, sizeof(struct coeff));
  memo_capacity = c;

  if (old_cap < c) {
    for (size_t i = old_cap; i < c; i++) mpz_init(bin_table[i].val);
  }
}

static void binomial(mpz_t dest, int n, int k) {
  if (memoisation_mode == SET) {
    if (memo_size == memo_capacity){
      const size_t new_cap = memo_capacity < 128 ? 256 : memo_capacity * 2;
      realloc_bin_table(new_cap);
    }
    bin_table[memo_size].n = n;
    bin_table[memo_size].k = k;
    mpz_bin_uiui(bin_table[memo_size].val, n, k);
  } else {
    assert(bin_table[memo_size].n == n);
    assert(bin_table[memo_size].k == k);
  }
  mpz_set(dest, bin_table[memo_size].val);
  memo_size++;
}

/*
 * McCaffrey algorithm with memoisation 
 */

void unrank_combinadics_memo(int* dest, int n, int k, const mpz_t rank) {
  // u <- binomial(n, k) - 1 - rank
  mpz_t u;
  mpz_init(u);
  // mpz_bin_uiui(u, n, k);
  binomial(u, n, k);
  mpz_sub_ui(u, u, 1);
  mpz_sub(u, u, rank);

  mpz_t b;
  mpz_init(b);

  int v = n;

  for (int i = 0; i < k; i++) {
    v--;
    // mpz_bin_uiui(b, v, k - i);
    binomial(b, v, k - i);
    while (mpz_cmp(u, b) < 0) {
      v--;
      // mpz_bin_uiui(b, v, k - i);
      binomial(b, v, k - i);
    }
    mpz_sub(u, u, b);
    dest[i] = n - 1 - v;
  }

  mpz_clear(u);
  mpz_clear(b);
}

/*
 * Buckles algorithm with memoisation 
 */

void unrank_combinadics2_memo(int* dest, int n, int k, const mpz_t rank) {
  // Special case for k = 1 since Buckles algorithm does not handle this case
  if (k == 1) {
    dest[0] = mpz_get_si(rank);
    return;
  }

  mpz_t r, b;
  mpz_init(r);
  mpz_init(b);

  int j = -1;
  for (int i = 0; i < k - 1; i++) {
    while (1) {
      j++;
      // mpz_bin_uiui(b, n - j - 1, k - i - 1);
      binomial(b, n - j - 1, k - i - 1);
      mpz_add(r, r, b);
      if (mpz_cmp(rank, r) < 0) break;
    }
    dest[i] = j;
    mpz_sub(r, r, b);
  }
  mpz_sub(r, rank, r);
  dest[k - 1] = j + mpz_get_si(r) + 1;

  mpz_clear(b);
  mpz_clear(r);
}

/*
 * Factoridcs-based algorithm with memoisation 
 */

void unrank_factoradics_memo(int* dest, int n, int k, const mpz_t rank) {
  mpz_t u, b;

  mpz_init(b);
  mpz_init_set(u, rank);

  int m = 0;
  int i = 0;

  while (i < k) {
    // mpz_bin_uiui(b, n - 1 - m, k - 1 - i);
    binomial(b, n - 1 - m, k - 1 - i);
    if (mpz_cmp(u, b) < 0) {
      dest[i] = m + i;
      i++;
      n--;
    } else {
      mpz_sub(u, u, b);
      m++;
    }
  }

  mpz_clear(u);
  mpz_clear(b);
}

/*
 * Recursive method
 */

static void _unrank_recursive_method_memo(int* dest, int n, int k, mpz_t rank) {
  // Base case 1: the empty subset
  if (k == 0) return;

  // Base case 2: the "full" subset: fill dest with {0, 1, 2, …, n-1}
  if (n == k) {
    for (int i = 0; i < k; i++) dest[i] = n - i - 1;
    return;
  }

  // Decide whether the last element of dest should be (n-1) or < (n-1).
  mpz_t binom;
  mpz_init(binom);
  // mpz_bin_uiui(binom, n - 1, k - 1);
  binomial(binom, n - 1, k - 1);
  if (mpz_cmp(rank, binom) < 0) {
    // {n-1} + unrank(n - 1, k - 1, rank)
    _unrank_recursive_method_memo(dest + 1, n - 1, k - 1, rank);
    dest[0] = n - 1;
  } else {
    // unrank(n - 1, k, rank - binom)
    mpz_sub(rank, rank, binom);
    _unrank_recursive_method_memo(dest, n - 1, k, rank);
  }
  mpz_clear(binom);
}

void unrank_recursive_method_memo(int* dest, int n, int k, const mpz_t rank) {
  // Co-lex unranking
  mpz_t r;
  mpz_init_set(r, rank);
  _unrank_recursive_method_memo(dest, n, k, r);

  // Fix the ordering
  for (int i = 0; i < k; i++) dest[i] = n - 1 - dest[i];

  mpz_clear(r);
}


/*
 * CLI
 */

static void run(FILE* fd, gmp_randstate_t s, unrank_algo_t f,
                int N, int step, int repeat) {
  // Allocate the destination array once and for all with enough
  // space for all the possible values of k.
  int* dest = calloc(N, sizeof(int));

  mpz_t* ranks = calloc(repeat, sizeof(mpz_t));
  for (int i = 0; i < repeat; i++) mpz_init(ranks[i]);

  for (int n = step; n < N; n += step) {
    const int k = n / 2;
    get_random_ranks(ranks, s, n, k, repeat);

    // Precompute the coefficients
    memoisation_mode = SET;
    memo_size = 0;
    for (int i = 0; i < repeat; i++) {
      f(dest, n, k, ranks[i]);
    }

    // Re-run with pre-computed coefficients
    memoisation_mode = GET;
    memo_size = 0;
    const clock_t pre = clock();
    for (int i = 0; i < repeat; i++) {
      f(dest, n, k, ranks[i]);
    }
    const clock_t post = clock();

    fprintf(fd, "%d %lf\n", n, (double)(post - pre) / CLOCKS_PER_SEC);
  }

  for (int i = 0; i < repeat; i++) mpz_clear(ranks[i]);
  free(dest);
  free(ranks);
}

name_algo_pair memo_algos[] = {
  {.name = "combinadics_memo", .func = unrank_combinadics_memo},
  {.name = "combinadics2_memo", .func = unrank_combinadics2_memo},
  {.name = "factoradics_memo", .func = unrank_factoradics_memo},
  {.name = "recursive_method_memo", .func = unrank_recursive_method_memo},
};

static const int nb_algos = sizeof(memo_algos) / sizeof(name_algo_pair);

static int usage_memo(char progname[]) {
  fprintf(stderr, "usage: %s ALGO\n", progname);
  fprintf(stderr, "ALGO:  algorithm to benchmark, possible values:");

  for (unsigned int i = 0; i < nb_algos; i++) {
    if (i % 3 == 0) fprintf(stderr, "\n       ");
    else fprintf(stderr, " ");
    fprintf(stderr, "%s", memo_algos[i].name);
    if (i < nb_algos - 1) fprintf(stderr, ",");
  }
  fprintf(stderr, "\n");

  return 1;
}

static unrank_algo_t find_algo_memo(char name[]) {
 for (unsigned int i = 0; i < nb_algos; i++) {
    if (strcmp(name, memo_algos[i].name) == 0)
      return memo_algos[i].func;
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  if (argc != 2) return usage_memo(argv[0]);

  // Choose which algorithm to benchmark
  unrank_algo_t algo = find_algo_memo(argv[1]);
  if (algo == NULL) {
    fprintf(stderr, "Unknown algorithm: %s\n", argv[1]);
    return usage_memo(argv[0]);
  }

  // Initialise the RNG
  gmp_randstate_t state;
  gmp_randinit_default(state);
  gmp_randseed_ui(state, 0x9244ce618857cb76L);

  // Bench!
  run(stdout, state, algo, 10000, 500, 200);

  return 0;
}
