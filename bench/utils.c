#include <stdio.h>
#include <string.h>

#include <gmp.h>

#include "utils.h"
#include "../combunrank.h"

void get_random_ranks(mpz_t* r, gmp_randstate_t s, int n, int k, int nb) {
  mpz_t binom;
  mpz_init(binom);
  mpz_bin_uiui(binom, n, k);

  for (int i = 0; i < nb; i++) {
    mpz_urandomm(r[i], s, binom);
  }

  mpz_clear(binom);
}

static const unsigned int nb_algos = sizeof(unrank_algo_list) / sizeof(name_algo_pair);

int usage(char progname[]) {
  fprintf(stderr, "usage: %s ALGO\n", progname);
  fprintf(stderr, "ALGO:  algorithm to benchmark, possible values:");

  for (unsigned int i = 0; i < nb_algos; i++) {
    if (i % 3 == 0) fprintf(stderr, "\n       ");
    else fprintf(stderr, " ");
    fprintf(stderr, "%s", unrank_algo_list[i].name);
    if (i < nb_algos - 1) fprintf(stderr, ",");
  }
  fprintf(stderr, "\n");

  return 1;
}

int find_algo(char name[]) {
  for (unsigned int i = 0; i < nb_algos; i++) {
    if (strcmp(name, unrank_algo_list[i].name) == 0)
      return i;
  }
  return -1;
}
