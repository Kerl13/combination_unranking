#include <stdio.h>
#include <malloc.h>
#include <time.h> // clock, clock_t, CLOCKS_PER_SEC
#include <string.h> // strcmp
#include <gmp.h>

#include "../combunrank.h"
#include "utils.h"


static void run(FILE* fd, gmp_randstate_t s, unrank_algo_t f, int n, int step, int repeat) {
  // Allocate the destination array once and for all with enough
  // space for all the possible values of k.
  int* dest = calloc(n, sizeof(int));

  mpz_t* ranks = calloc(repeat, sizeof(mpz_t));
  for (int i = 0; i < repeat; i++) mpz_init(ranks[i]);

  for (int k = step; k < n; k += step) {
    get_random_ranks(ranks, s, n, k, repeat);

    clock_t pre = clock();
    for (int i = 0; i < repeat; i++) {
      f(dest, n, k, ranks[i]);
    }
    clock_t post = clock();

    fprintf(fd, "%d %lf\n", k, (double)(post - pre) / CLOCKS_PER_SEC);
  }

  for (int i = 0; i < repeat; i++) mpz_clear(ranks[i]);
  free(dest);
  free(ranks);
}

int main(int argc, char* argv[]) {
  if (argc != 2) return usage(argv[0]);

  // Choose which algorithm to benchmark
  unrank_algo_t algo = find_algo(argv[1]);
  if (algo == NULL) {
    fprintf(stderr, "Unknown algorithm: %s\n", argv[1]);
    return usage(argv[0]);
  }

  // Initialise the RNG
  gmp_randstate_t state;
  gmp_randinit_default(state);
  gmp_randseed_ui(state, 0x9244ce618857cb76L);

  // Bench!
  run(stdout, state, algo, 10000, 250, 500);

  return 0;
}
