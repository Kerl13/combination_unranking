// Wee CLI tool to sample combinations

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>
#include "combunrank.h"

typedef void (*algo)(int*, int, int, const mpz_t);

typedef struct {
  char* name;
  algo func;
} pair;


static const pair algorithms[] = {
  {.name = "recursive_method", .func = unrank_recursive_method},
  {.name = "recursive_method_naive", .func = unrank_recursive_method_naive},
  {.name = "factoradics", .func = unrank_factoradics},
  {.name = "factoradics_naive", .func = unrank_factoradics_naive},
};

static algo get_algorithm(char name[]) {
  for (unsigned int i = 0; i < sizeof(algorithms) / sizeof(pair); i++) {
    if (strcmp(algorithms[i].name, name) == 0)
      return algorithms[i].func;
  }

  return NULL;
}


// ---
// CLI parsing
// ---

typedef struct {
  int n;
  int k;
  mpz_t rank;
  char* algo;
} cli_options;

int usage(char progname[]) {
  fprintf(stderr, "usage: %s -n N -k K -r RANK [-a ALGORITHM]\n", progname);
  return 1;
}

static int parse_pos_int(char* s) {
  char* endptr;
  long res = strtol(s, &endptr, 10);
  if (endptr == s)
    return -1;
  else if (res > INT_MAX) {
    fprintf(stderr, "NB is too big (> %d).\n", INT_MAX);
    return -1;
  } else if (res <= 0) {
    fprintf(stderr, "NB must be positive.\n");
    return -1;
  } else
    return (int)res;
}


cli_options cli_parse(int argc, char* argv[]) {
  cli_options opts;
  mpz_init(opts.rank);
  opts.algo = "recursive_method";

  int c;
  while ((c = getopt(argc, argv, "n:k:r:a:")) != -1) {
    switch (c) {
      case 'n':
        opts.n = parse_pos_int(optarg);
        if (opts.n < 0) exit(usage(argv[0]));
        break;
      case 'k':
        opts.k = parse_pos_int(optarg);
        if (opts.k < 0) exit(usage(argv[0]));
        break;
      case 'r':
        if (mpz_set_str(opts.rank, optarg, 10) == -1) exit(usage(argv[0]));
        break;
      case 'a':
        opts.algo = optarg;
        break;
      default:
        abort();
    }
  }

  return opts;
}

int main(int argc, char* argv[]) {
  cli_options opts = cli_parse(argc, argv);

  // Sanity checks
  if (opts.k > opts.n) {
    fprintf(stderr, "k too big, should be at most n\n");
    return usage(argv[0]);
  }
  mpz_t binom;
  mpz_init(binom);
  mpz_bin_uiui(binom, opts.n, opts.k);
  if (mpz_sgn(opts.rank) < 0 || mpz_cmp(opts.rank, binom) >= 0) {
    fprintf(stderr, "Invalid rank, should be in [0; binomial(n, k)]\n");
    return usage(argv[0]);
  }

  // Select the algorithm
  algo algo = get_algorithm(opts.algo);
  if (algo == NULL) return usage(argv[0]);

  int* comb = calloc(opts.k, sizeof(int));
  algo(comb, opts.n, opts.k, opts.rank);

  for (int i = 0; i < opts.k - 1; i++) {
    printf("%d, ", comb[i]);
  }
  if (opts.k > 0) printf("%d", comb[opts.k - 1]);
  printf("\n");

  free(comb);
  return 0;
}
